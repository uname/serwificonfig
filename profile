#/usr/bin/env sh
OPENWRT_HOME=${HOME}/Programming/OpenWRT-dev
TOOLCHAIN_HOME=${OPENWRT_HOME}/staging_dir/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2
export PATH=${TOOLCHAIN_HOME}/bin:$PATH
export STAGING_DIR=${TOOLCHAIN_HOME}
