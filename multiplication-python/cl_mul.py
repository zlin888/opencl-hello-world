import pyopencl as cl
import numpy
from typing import Type


class CL:
    def __init__(self):
        self.ctx = cl.create_some_context()
        self.queue = cl.CommandQueue(self.ctx)

    def loadProgram(self, filename):
        # read in the OpenCL source file as a string
        with open(filename, 'r') as f:
            code = "".join(f.readlines())
            print(code)
            self.program = cl.Program(self.ctx, code).build()

    def popCorn(self):
        mf = cl.mem_flags

        # initialize client side (CPU) arrays
        self.a = numpy.array(range(10), dtype=numpy.float32)
        self.b = numpy.array(range(10), dtype=numpy.float32)

        # create OpenCL buffers
        self.a_buf = cl.Buffer(self.ctx, mf.READ_ONLY |
                               mf.COPY_HOST_PTR, hostbuf=self.a)
        self.b_buf = cl.Buffer(self.ctx, mf.READ_ONLY |
                               mf.COPY_HOST_PTR, hostbuf=self.b)
        self.c_buf = cl.Buffer(self.ctx, mf.WRITE_ONLY, self.b.nbytes)

    def execute(self):
        self.program.mul(self.queue, self.a.shape, None,
                         self.a_buf, self.b_buf, self.c_buf)
        c = numpy.empty_like(self.a)
        cl.enqueue_copy(self.queue, c, self.c_buf).wait()
        print("a", self.a)
        print("b", self.b)
        print("c", c)


if __name__ == "__main__":
    mul = CL()
    mul.loadProgram("mul.cl")
    mul.popCorn()
    mul.execute()
