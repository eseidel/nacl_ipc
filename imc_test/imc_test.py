#!/usr/bin/env python

import os
import subprocess
import socket
import sys


def main():
    if len(sys.argv) < 2:
        print "Usage: %s NEXE_PATH" % sys.argv[0]
        sys.exit(1)

    sel_ldr_path = os.getenv("LDR")
    if not sel_ldr_path:
        print "LDR environment variable required"
        sys.exit(1)

    nexe_path = sys.argv[1]
    print "NEXE: %s" % nexe_path

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
        "./imc_test_host",
        "%s" % host_socket.fileno(),
    ]
    nacl_process = subprocess.Popen(nacl_args)
    host_process = subprocess.Popen(host_args)

if __name__ == "__main__":
    main()
