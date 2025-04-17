#!/usr/bin/env python3

import os
import shutil
import subprocess
import sys
import platform
import zipfile
import tarfile
from pathlib import Path


def clean_directory(dir_path: str, dir_name: str) -> None:
    """Remove directory if it exists."""
    if os.path.exists(dir_path):
        print(f"Deleting {dir_name} directory {dir_path}...")
        try:
            shutil.rmtree(dir_path)
            print(f"{dir_name} directory deleted successfully.")
        except Exception as e:
            print(f"Error deleting {dir_name} directory: {e}")
            sys.exit(1)


def configure_project() -> None:
    """Configure the project using CMake."""
    print("\nConfiguring the project...")
    configure_cmd = [
        "cmake",
        "-DCMAKE_BUILD_TYPE=Release",
        "-S", ".",
        "-B", "build"
    ]
    try:
        subprocess.run(configure_cmd, check=True)
        print("Configuration completed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Configuration error: {e}")
        sys.exit(1)


def build_project() -> None:
    """Build the project using CMake."""
    print("\nBuilding the project...")
    build_cmd = ["cmake", "--build", "build", "--config", "Release"]
    try:
        subprocess.run(build_cmd, check=True)
        print("Build completed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Build error: {e}")
        sys.exit(1)


def get_executable_path(build_dir: str) -> str:
    """Determine the correct executable path based on platform and build type"""
    executable_name = "pong.exe" if platform.system() == "Windows" else "pong"
    
    if platform.system() == "Windows":
        # Для MSVC сборки ищем в Debug/Release подпапках
        for build_type in ["Release", "Debug"]:
            path = os.path.join(build_dir, build_type, executable_name)
            if os.path.exists(path):
                return path
        # Если не нашли в подпапках, пробуем стандартный путь
        return os.path.join(build_dir, executable_name)
    else:
        # Для Linux/MacOS стандартный путь
        return os.path.join(build_dir, executable_name)



def prepare_deployment_files(build_dir: str, deploy_dir: str) -> None:
    """Prepare deployment files by copying executable and resources"""
    executable_path = get_executable_path(build_dir)
    
    if not os.path.exists(executable_path):
        raise FileNotFoundError(
            f"Executable not found at {executable_path}. "
            "Please check build configuration."
        )

    # Остальной код остается прежним
    deploy_bin = os.path.join(deploy_dir, "bin")
    os.makedirs(deploy_bin, exist_ok=True)
    shutil.copy2(executable_path, deploy_bin)
    
    resources_src = os.path.join(os.getcwd(), "resources")
    deploy_resources = os.path.join(deploy_dir, "resources")
    
    if os.path.exists(resources_src):
        shutil.copytree(resources_src, deploy_resources, dirs_exist_ok=True)
    else:
        print("Warning: 'resources' directory not found!")


def create_archive(deploy_dir: str) -> str:
    """Create deployment archive based on the operating system."""
    archive_name = "pong_windows" if platform.system() == "Windows" else "pong_linux"
    archive_ext = "zip" if platform.system() == "Windows" else "tar.gz"
    archive_path = os.path.join(os.getcwd(), f"{archive_name}.{archive_ext}")

    # Remove existing archive if present
    if os.path.exists(archive_path):
        os.remove(archive_path)

    # Create archive
    if platform.system() == "Windows":
        with zipfile.ZipFile(archive_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
            for root, _, files in os.walk(deploy_dir):
                for file in files:
                    file_path = os.path.join(root, file)
                    arcname = os.path.relpath(file_path, deploy_dir)
                    zipf.write(file_path, arcname)
    else:
        with tarfile.open(archive_path, "w:gz") as tar:
            tar.add(deploy_dir, arcname=os.path.basename(deploy_dir))

    return archive_path


def main():
    # Clean up previous build and deploy directories
    clean_directory(os.path.join(os.getcwd(), "deploy"), "deploy")
    clean_directory(os.path.join(os.getcwd(), "build"), "build")

    # Build the project
    configure_project()
    build_project()

    # Prepare deployment
    print("\nPreparing deployment archive...")
    deploy_dir = os.path.join(os.getcwd(), "deploy")
    os.makedirs(deploy_dir, exist_ok=True)
    
    prepare_deployment_files("build", deploy_dir)
    archive_path = create_archive(deploy_dir)
    
    print(f"\nDeployment archive created: {archive_path}")


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}")
        if platform.system() == "Windows":
            os.system("pause")
    else:
        if platform.system() == "Windows":
            input("Press Enter to exit...")