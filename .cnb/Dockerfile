FROM ubuntu:22.04 AS ci

ARG DEBIAN_FRONTEND ENV_MIRROR

RUN \
--mount=type=bind,from=zoo0/toolkit:0.0.1,src=/scm,target=/scm \
--mount=type=cache,target=/var/lib/apt/lists,sharing=private \
/scm/update-apt-source $ENV_MIRROR && \
apt-get update && \
apt-get -y install curl git git-lfs xz-utils

RUN \
git clone -b musl-cross https://cnb.cool/beilinmo/CNB-NightStar/is-depot /musl && \
mkdir -p /opt && \
tar -xf /musl/x86_64-linux-musl-cross.tgz -C /opt && \
tar -xf /musl/aarch64-linux-musl-cross.tgz -C /opt && \
tar -xf /musl/riscv64-linux-musl-cross.tgz -C /opt && \
rm -fr /musl

ENV PATH=$PATH:/opt/x86_64-linux-musl-cross/bin:/opt/aarch64-linux-musl-cross/bin:/opt/riscv64-linux-musl-cross/bin

