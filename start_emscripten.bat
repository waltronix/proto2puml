echo off

SET SOURCE_DIR=%~dp0
SET TARGET_DIR=/mnt/proto2puml
SET DOCKER_REGISTRY_BASE=
SET DOCKER_IMAGE=trzeci/emscripten:sdk-tag-1.38.43-64bit

rem echo(
rem echo   pulling docker image:  %DOCKER_IMAGE%
rem )
rem SET DOCKER_COMMAND=docker pull %DOCKER_REGISTRY_BASE%%DOCKER_IMAGE%
rem echo %DOCKER_COMMAND%
rem %DOCKER_COMMAND%

echo(
echo   running docker image: %DOCKER_IMAGE%
echo       mounting   -   %SOURCE_DIR%
echo             to   -   %TARGET_DIR%
)

SET DOCKER_COMMAND=docker run -ti --rm --mount type=bind,source=%SOURCE_DIR%,target=%TARGET_DIR% --workdir %TARGET_DIR% %DOCKER_REGISTRY_BASE%%DOCKER_IMAGE% bash
echo %DOCKER_COMMAND%
%DOCKER_COMMAND%

