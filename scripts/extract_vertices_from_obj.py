import argparse

def read_vertices_list(obj_file):
    result = []
    cur_line = None
    while cur_line := obj_file.readline():
        splitted = cur_line.split()

        if splitted[0] != "v":
            continue

        result.append((
            float(splitted[1]),
            float(splitted[2]),
            float(splitted[3])
        ))

    return result

def read_normals_list(obj_file):
    result = []
    cur_line = None
    while cur_line := obj_file.readline():
        splitted = cur_line.split()

        if splitted[0] != "vn":
            continue

        result.append((
            float(splitted[1]),
            float(splitted[2]),
            float(splitted[3])
        ))

    return result

def parse_face_int(string):
    if not string:
        return 0
    else:
        return int(string)

def read_faces_list(obj_file):
    result = []
    cur_line = None
    while cur_line := obj_file.readline():
        splitted = cur_line.split()

        if splitted[0] != "f":
            continue

        if len(splitted) > 4:
            raise RuntimeError("""Looks like this .obj file contains faces with more that 3 vertices.
                These are not supported, please make sure that the model is triangulated.""")

        cur_result = [None] * 3
        for i in range(3):
            cur_splitted = splitted[i + 1].split("/")
            cur_result[i] = (
                parse_face_int(cur_splitted[0]),
                parse_face_int(cur_splitted[1]),
                parse_face_int(cur_splitted[2])
            )

        result.append(cur_result)

    return result

def combine_vertices(vertices_list, faces_list):
    result = []
    for face in faces_list:
        result.extend(vertices_list[face[0][0] - 1])
        result.extend(vertices_list[face[1][0] - 1])
        result.extend(vertices_list[face[2][0] - 1])

    return result

def combine_normals(normals_list, faces_list):
    result = []
    for face in faces_list:
        triangle_normals = [None] * 3
        triangle_normals[0] = normals_list[face[0][2] - 1]
        triangle_normals[1] = normals_list[face[1][2] - 1]
        triangle_normals[2] = normals_list[face[2][2] - 1]

        normal_avg = [
            (triangle_normals[0][0] + triangle_normals[1][0] + triangle_normals[2][0]) / 3,
            (triangle_normals[0][1] + triangle_normals[1][1] + triangle_normals[2][1]) / 3,
            (triangle_normals[0][2] + triangle_normals[1][2] + triangle_normals[2][2]) / 3,
        ]

        result.extend(normal_avg)

    return result

def float_to_fixed32_hex(number):
    int_number = int(number * 65536.0)

    if int_number >= 2**31 or int_number < -2**31:
        raise ValueError("One of the numbers is out of range of fixed32.")

    if int_number < 0:
        int_number = (1 << 32) + int_number

    return "0x" + hex(int_number)[2:].zfill(8)

def dump_vectors_in_fixed32_hex(identation_spaces, vertices):
    result = ""
    for i in range(len(vertices)):
        if i % 3 == 0:
            result += " " * identation_spaces
            result += "{"

        end = None
        if i == len(vertices) - 1:
            end = "}"
        elif (i + 1) % 3 == 0:
            end = "}, \n"
        else:
            end = ", "

        result += float_to_fixed32_hex(vertices[i])
        result += end

    return result

def form_a_header_file(vertices, normals):
    return """
#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "math/vec3.h"

#define TRIANGLES_COUNT {}

static const struct vec3 VERTICES[TRIANGLES_COUNT * 3] = {{
{}
}};

static const struct vec3 NORMALS[TRIANGLES_COUNT] = {{
{}
}};

#endif // MESH_DATA_H
    """.format(
        len(vertices) // 9,
        dump_vectors_in_fixed32_hex(4, vertices),
        dump_vectors_in_fixed32_hex(4, normals)
    )

def main():
    arg_parser = argparse.ArgumentParser(description="""
    This script reads a Wavefront OBJ file and dumps model vertices and normals (one normal per triangle) in hex fixed 16.16 format.
    """, formatter_class=argparse.RawDescriptionHelpFormatter
        )
    arg_parser.add_argument("--input", "-i", metavar="<path to .obj file>", type=str, required=True)
    args = arg_parser.parse_args()

    obj_file = open(args.input, "rt")

    vertices_list = read_vertices_list(obj_file)
    obj_file.seek(0)
    normals_list = read_normals_list(obj_file)
    obj_file.seek(0)
    faces_list = read_faces_list(obj_file)

    vertices = combine_vertices(vertices_list, faces_list)
    normals = combine_normals(normals_list, faces_list)

    print(form_a_header_file(vertices, normals))

if __name__ == "__main__":
    main()
