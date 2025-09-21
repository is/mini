# ===
variable "CNB_REPO_SLUG_LOWERCASE" {}
variable "CNB_DOCKER_REGISTRY" {}
variable "ENV_MIRROR" {
  default = "tencent"
}


# ===
group "default" {
  targets = ["ci"]
}


# ===
target "_common" {
  args = {
    DEBIAN_FRONTEND = "noninteractive"
    BASE_IMAGE = "ubuntu:25.04"
    ENV_MIRROR = "${ENV_MIRROR}"
    MC_URL = "https://cnb.cool/beilinmo/CNB-NightStar/is-depot/-/lfs/01f866e9c5f9b87c2b09116fa5d7c06695b106242d829a8bb32990c00312e891"
  }

  contexts = {
    scm-toolkit = "docker-image://docker.cnb.cool/io8/e1/toolkit:0.0.2"
  }
}

target "toolkit" {
  inherits = ["_common"]
  context = ".cnb"
  tags = ["docker.cnb.cool/beilinmo/cnb-nightstar/armory/mini-toolkit:0.0.1"]  
  target = "toolkit"
}

target "ci" {
  inherits = ["_common"]
  context = ".cnb"
  tags = ["docker.cnb.cool/beilinmo/cnb-nightstar/armory/mini-ci:0.0.1"]
}