import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-i", action = "store", dest = "input_path", default = ".")
parser.add_argument("-o", action = "store", dest = "output_path", default = ".")

def convert(filePath, fileName, outputPath) :
    name = os.path.splitext(fileName)[0]

    fin = open(filePath + "/" + fileName, "r")
    # fout = open(os.getcwd() + "/" + os.path.splitext(fileName)[0] + ".hpp", "w")
    fout = open(outputPath + "/" + name + ".hpp", "w")

    source = fin.read()
    fout.write("#ifndef OPENCL_KERNEL_{0}_HPP\n".format(name))
    fout.write("#define OPENCL_KERNEL_{0}_HPP\n\n".format(name))
    fout.write("const char* {0}_SourceCode =\n".format(name))
    fout.write('R"(\n')
    fout.write(source + "\n")
    fout.write(')";\n\n')
    fout.write("#endif // OPENCL_KERNEL_{0}_HPP".format(name))

    fin.close()
    fout.close()


def scan(filePath, outputPath, suffix) :
    if(not os.path.isdir(filePath)) :
        exit(-1)
    for fileName in os.listdir(filePath) :
        if(os.path.isdir(filePath + "/" + fileName)) :
            scan(filePath + "/" + fileName, outputPath, suffix)
        else :
            if(fileName.endswith(suffix)) :
                # convert to header file
                convert(filePath, fileName, outputPath)

if __name__ == "__main__" :
    args = parser.parse_args();
    # scan(os.path.split(os.path.realpath(__file__))[0], "cl")
    scan(args.input_path, args.output_path, "cl")
