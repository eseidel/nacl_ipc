#!/usr/bin/env python

import subprocess
import socket

sel_ldr_path = "/Projects/native_client/native_client/scons-out/opt-mac-x86-32/obj/src/trusted/service_runtime/sel_ldr"
nexe_path = "imc_test.nexe"

def main():
    our_socket, their_socket = socket.socketpair(socket.AF_UNIX, socket.SOCK_DGRAM)
    nacl_app_imc_id = 10

    # This example matches http://www.chromium.org/nativeclient/life-of-sel_ldr
    args = [
        sel_ldr_path,
        # "-v", "-v",
        "-i",
        "%s:%s" % (nacl_app_imc_id, their_socket.fileno()),
        # "-X",
        # "%s" % nacl_app_imc_id,
        "--",
        nexe_path
    ]
    print args
    p = subprocess.Popen(args)
    print our_socket.recv(100)

if __name__ == "__main__":
    main()
