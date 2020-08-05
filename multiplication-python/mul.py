from typing import List, Tuple, Type, NamedTuple
import random
from collections import namedtuple


# class Matrix:
#     def __init__(self, dim_row: int, dim_col: int, elements: List[float] = None):
#         self.matrix = get_matrix(dim_row, dim_col, elements)
#         self.dim_row = dim_row
#         self.dim_col = dim_col

#     @staticmethod
#     def get_matrix(dim_row: int, dim_col: int, elements: List[float] = None) -> List[List[int]]:
#         if not elements:
#             def get_random(): return random.random() * 100
#             return [[get_random() for j in range(dim_col)] for i in range(dim_row)]
#         else:
#             assert dim_row * \
#                 dim_col == len(elements), 'dim should match elements'
#             return [[elements[i * dim_col + j] for j in range(dim_col)] for i in range(dim_row)]

#     def __getitem__(self, key: int) -> List[float]:
#         return self.matrix[key]


class Matrix(NamedTuple):
    dim_row: int
    dim_col: int
    matrix: List[List[int]]

    def __getitem__(self, key: int) -> List[float]:
        return self.matrix[key]

    def __mul__(self, other):
        return self.mul(self, other)

    def __rmul__(self, other):
        return self.mul(other, self)

    @staticmethod
    def mul(a, b):
        assert a.dim_col == b.dim_row, "l_matrix's dim_col should match r_matrix's dim_row"
        c = [[0 for j in range(b.dim_col)] for i in range(a.dim_row)]
        # n*k * k*m = n*m
        for i in range(a.dim_row):
            for k in range(a.dim_col):  # a.dim_col == b.dim_row (k=k)
                for j in range(b.dim_col):
                    c[i][j] += a[i][k] * b[k][j]
        return Matrix(a.dim_row, b.dim_col, c)


def get_matrix(dim_row: int, dim_col: int, elements: List[float] = None) -> Type[Matrix]:
    if not elements:
        def get_random(): return random.random() * 100
        return Matrix(dim_row, dim_col,
                      [[get_random() for j in range(dim_col)] for i in range(dim_row)])
    else:
        assert dim_row * \
            dim_col == len(elements), 'dim should match elements'
        return Matrix(dim_row, dim_col,
                      [[elements[i * dim_col + j] for j in range(dim_col)] for i in range(dim_row)])

print(get_matrix(5, 12) * get_matrix(12, 3))
