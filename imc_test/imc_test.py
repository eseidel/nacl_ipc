#!/usr/bin/env python

import subprocess
import socket

sel_ldr_path = "/Projects/native_client/native_client/scons-out/opt-mac-x86-32/obj/src/trusted/service_runtime/sel_ldr"
nexe_path = "imc_test_x86_32.nexe"

def main():
    our_socket, their_socket = socket.socketpair(socket.AF_UNIX, socket.SOCK_DGRAM)
    
    args = [
        sel_ldr_path,
        "-i",
        "%s:%s" % (our_socket.fileno(), 10),
        "-X",
        "10",
        "--",
        nexe_path
    ]
    print args
    p = subprocess.Popen(args)
    their_socket.send("Test")

if __name__ == "__main__":
    main()
