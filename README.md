# DupliTrace - Duplicate file finder/reporter

Work in progress

## Depencies

### Dependencies - header-only libraries
The following header-only libraries are required by the project, you will need to clone the repository and then add the environment variable.

Dependency | Repository | Environment Variable
---        | ---        | ---
ArgParse | https://github.com/p-ranav/argparse | DUPLITRACE_ARGPARSE_INCLUDE
spdlog   | https://github.com/gabime/spdlog | DUPLITRACE_SPDLOG_INCLUDE

### Dependencies - Embedded libraries

Dependency | Repository
---        | ---
iniReader  | https://github.com/benhoyt/inih

## Docker Images

Docker Images are stored at Docker Hub and can be found [Here](https://hub.docker.com/repository/docker/swatkat1977).

Bleeding-edge (nightly) builds are created from the main branch. They should not be used in production as they are not production-ready and could contain bugs.
