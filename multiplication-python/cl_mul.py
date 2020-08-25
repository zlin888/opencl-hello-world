import pyopencl as cl
import numpy
from typing import Type


class CL:
    def __init__(self):
        self.ctx = cl.create_some_context()
        self.queue = cl.CommandQueue(self.ctx)

    def load_program(self, filename):
        # read in the OpenCL source file as a string
        with open(filename, 'r') as f:
            code = "".join(f.readlines())
            print(code)
            self.program = cl.Program(self.ctx, code).build()

    def pop_corn(self):
        mf = cl.mem_flags

        # initialize client side (CPU) arrays
        self.a = numpy.array([range(10)
                              for i in range(2)], dtype=numpy.float32)
        self.b = numpy.array([range(5)
                              for i in range(10)], dtype=numpy.float32)
        self.c = numpy.array([range(5)
                              for i in range(2)], dtype=numpy.float32)
        print(self.a)
        print(self.b)

        # create OpenCL buffers
        self.a_buf = cl.Buffer(self.ctx, mf.READ_ONLY |
                               mf.COPY_HOST_PTR, hostbuf=self.a)
        self.b_buf = cl.Buffer(self.ctx, mf.READ_ONLY |
                               mf.COPY_HOST_PTR, hostbuf=self.b)
        self.c_buf = cl.Buffer(self.ctx, mf.WRITE_ONLY, self.c.nbytes)

    def execute(self):
        kernel = self.program.mul
        kernel.set_args(self.a_buf, self.b_buf, self.c_buf, numpy.int32(2), numpy.int32(5), numpy.int32(10))
        cl.enqueue_nd_range_kernel(self.queue, kernel, (2, 5), None)

        c = numpy.empty_like(self.a.dot(self.b))
        cl.enqueue_copy(self.queue, c, self.c_buf).wait()
        print("a", self.a)
        print("b", self.b)
        print("c", c)


if __name__ == "__main__":
    mul = CL()
    mul.load_program("mul.cl")
    mul.pop_corn()
    mul.execute()
