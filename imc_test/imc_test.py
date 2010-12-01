#!/usr/bin/env python

import subprocess
import socket

sel_ldr_path = "/Projects/native_client/native_client/scons-out/opt-mac-x86-32/obj/src/trusted/service_runtime/sel_ldr"
nexe_path = "imc_test.nexe"

def main():
    host_socket, nacl_socket = socket.socketpair(socket.AF_UNIX, socket.SOCK_DGRAM)
    nacl_app_imc_id = 10

    # This example matches http://www.chromium.org/nativeclient/life-of-sel_ldr
    nacl_args = [
        sel_ldr_path,
        # "-v", "-v",
        "-i",
        "%s:%s" % (nacl_app_imc_id, nacl_socket.fileno()),
        # "-X",
        # "%s" % nacl_app_imc_id,
        "--",
        nexe_path
    ]
    host_args = [
        "./a.out",
        "%s" % host_socket.fileno(),
    ]
    host_process = subprocess.Popen(host_args)
    nacl_process = subprocess.Popen(nacl_args)


if __name__ == "__main__":
    main()
