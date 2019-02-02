#!/usr/bin/python

import os

def link(folder):
    if os.path.exists(folder):
        if not os.path.exists(folder + "/Resources"):
            os.makedirs(folder + "/Resources")
            os.symlink("../Resources", folder + "/Resources/Engine")

link("../cmake-build-debug/bin")
link("../cmake-build-release/bin")
