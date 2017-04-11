###########################################################################
# Copyright 2015, 2016, 2017 IoT.bzh
#
# author: Fulup Ar Foll <fulup@iot.bzh>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################

# Project Info
# ------------------
set(NAME low-can-binding)
set(VERSION "1.0")
set(PRETTY_NAME "Low level CAN binding")
set(DESCRIPTION "Expose CAN Low Level APIs through AGL Framework")
set(URL "https://github.com/iotbzh/CAN_signaling")
set(PROJECT_ICON "icon.png")

# Compilation Mode (DEBUG, RELEASE)
# ----------------------------------
setc(CMAKE_BUILD_TYPE "DEBUG")

# PKG_CONFIG required packages
# -----------------------------
set (PKG_REQUIRED_LIST 
	json-c
	libsystemd
	afb-daemon
)

# Static constante definition
# -----------------------------
add_compile_options(-D_REENTRANT)
add_compile_options(-DPB_FIELD_16BIT)

# Print a helper message when every thing is finished
setc(CLOSING_MESSAGE "Test with: afb-daemon --ldpaths=\$\$(pwd)/package/lib --port=1234 --roothttp=\$\$(pwd)/package/htdocs --tracereq=common --token=\"\" --verbose")
# ----------------------------------------------------

# (BUG!!!) as PKG_CONFIG_PATH does not work [should be an env variable]
# ---------------------------------------------------------------------
setc(CMAKE_INSTALL_PREFIX ${HOME}/opt)
setc(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX}/lib64/pkgconfig ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
setc(LD_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib64 ${CMAKE_INSTALL_PREFIX}/lib)

# Optional dependencies order
# ---------------------------
#set(EXTRA_DEPENDENCIES_ORDER target1 target2 ...)

# Optional Extra global include path
# -----------------------------------
#set(EXTRA_INCLUDE_DIRS incdir1 incdir2 ...)

# Optional extra libraries
# -------------------------
set(EXTRA_LINK_LIBRARIES nanopb bitfield-c isotp-c uds-c openxc-message-format)

# Optional force binding installation
# ------------------------------------
# set(BINDINGS_INSTALL_PREFIX DestinationPath )

# Optional force binding Linking flag
# ------------------------------------
# set(BINDINGS_LINK_FLAG LinkOptions )


