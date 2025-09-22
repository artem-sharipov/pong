#!/usr/bin/env python3

import os
import shutil
import subprocess
import sys
import platform
import zipfile
import tarfile
from pathlib import Path


def find_resource_files(resources_dir="resources"):
    """Automatically find all resource files in the resources directory"""
    resource_files = []
    
    if not os.path.exists(resources_dir):
        print(f"Warning: Resources directory '{resources_dir}' not found")
        return resource_files
    
    # Supported file extensions for embedding
    supported_extensions = {'.png', '.wav'}
    
    for root, dirs, files in os.walk(resources_dir):
        for file in files:
            file_path = os.path.join(root, file)
            file_ext = os.path.splitext(file)[1].lower()
            
            if file_ext in supported_extensions:
                resource_files.append(file_path)
                print(f"Found resource: {file_path}")
    
    # Sort for consistent ordering
    resource_files.sort()
    return resource_files


def generate_embedded_resources():
    """Generate embedded_resources.h and embedded_resources.c from resource files"""
    print("\nGenerating embedded resources...")
    
    # Automatically find all resource files
    resources = find_resource_files()
    
    if not resources:
        print("No resource files found in 'resources' directory!")
        return
    
    build_dir = "build"
    include_dir = os.path.join(build_dir, "include")
    os.makedirs(include_dir, exist_ok=True)
    
    header_file = os.path.join(include_dir, "embedded_resources.h")
    source_file = os.path.join(build_dir, "embedded_resources.c")
    
    # Check if xxd is available
    if not shutil.which("xxd"):
        print("Error: xxd utility not found. Please install it.")
        sys.exit(1)
    
    # Generate header file
    with open(header_file, 'w') as hf:
        hf.write("// Embedded resources header\n")
        hf.write("// Automatically generated - do not edit manually\n\n")
        hf.write("#ifndef EMBEDDED_RESOURCES_H\n")
        hf.write("#define EMBEDDED_RESOURCES_H\n\n")
        hf.write("#ifdef __cplusplus\n")
        hf.write('extern "C" {\n')
        hf.write("#endif\n\n")
        
        hf.write("// Resource file declarations:\n")
        for resource in resources:
            resource_name = os.path.basename(resource)
            var_name = resource_name.replace('.', '_').replace('-', '_')
            hf.write(f"extern unsigned char {var_name}[];\n")
            hf.write(f"extern unsigned int {var_name}_len;\n")
        
        hf.write("\n// Function to get resource by filename\n")
        hf.write("const unsigned char* get_embedded_resource(const char* filename, unsigned int* size);\n")
        
        hf.write("\n#ifdef __cplusplus\n")
        hf.write("}\n")
        hf.write("#endif\n\n")
        hf.write("#endif // EMBEDDED_RESOURCES_H\n")
    
    # Generate source file with corrected variable names and lookup function
    with open(source_file, 'w') as sf:
        sf.write("// Embedded resources source\n")
        sf.write("// Automatically generated - do not edit manually\n\n")
        sf.write('#include "embedded_resources.h"\n')
        sf.write('#include <string.h>\n\n')
        
        sf.write("// Resource data:\n")
        resource_map = []
        
        for resource in resources:
            resource_name = os.path.basename(resource)
            short_var_name = resource_name.replace('.', '_').replace('-', '_')
            print(f"Processing {resource_name} as {short_var_name}...")
            
            # Generate C array using xxd
            try:
                result = subprocess.run(
                    ["xxd", "-i", resource],
                    capture_output=True,
                    text=True,
                    check=True
                )
                
                # Fix the variable names in the generated output
                output_lines = result.stdout.split('\n')
                fixed_output = []
                
                for line in output_lines:
                    if line.startswith('unsigned char') and '_' in line:
                        parts = line.split()
                        if len(parts) >= 3:
                            long_var_name = parts[2]
                            if long_var_name.endswith('[]'):
                                long_var_name = long_var_name[:-2]
                            line = line.replace(long_var_name, short_var_name)
                    elif line.startswith('unsigned int') and '_len' in line:
                        parts = line.split()
                        if len(parts) >= 3:
                            long_len_name = parts[2]
                            if long_len_name.endswith(';'):
                                long_len_name = long_len_name[:-1]
                            line = line.replace(long_len_name, f"{short_var_name}_len")
                    
                    fixed_output.append(line)
                
                sf.write(f"\n// {resource}\n")
                sf.write('\n'.join(fixed_output))
                sf.write("\n")
                
                # Add to resource map for lookup function
                resource_map.append((resource_name, short_var_name))
                
            except subprocess.CalledProcessError as e:
                print(f"Error processing {resource}: {e}")
                sys.exit(1)
        
        # Generate lookup function
        sf.write("\n// Resource lookup function\n")
        sf.write("const unsigned char* get_embedded_resource(const char* filename, unsigned int* size) {\n")
        
        for resource_name, var_name in resource_map:
            sf.write(f'    if (strcmp(filename, "{resource_name}") == 0) {{\n')
            sf.write(f'        if (size) *size = {var_name}_len;\n')
            sf.write(f'        return {var_name};\n')
            sf.write('    }\n')
        
        sf.write('    if (size) *size = 0;\n')
        sf.write('    return NULL;\n')
        sf.write('}\n')
    
    print(f"Embedded resources generated successfully. Processed {len(resources)} files.")
    for resource in resources:
        print(f"  - {resource}")


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
        for build_type in ["Release", "Debug"]:
            path = os.path.join(build_dir, build_type, executable_name)
            if os.path.exists(path):
                return path
        return os.path.join(build_dir, executable_name)
    else:
        return os.path.join(build_dir, executable_name)


def prepare_deployment_files(build_dir: str, deploy_dir: str) -> None:
    """Prepare deployment files by copying executable directly to deploy directory"""
    executable_path = get_executable_path(build_dir)
    
    if not os.path.exists(executable_path):
        raise FileNotFoundError(
            f"Executable not found at {executable_path}. "
            "Please check build configuration."
        )

    # Copy executable directly to deploy directory (no bin subfolder)
    shutil.copy2(executable_path, deploy_dir)
    
    print("Resources are embedded in the executable - standalone deployment!")


def create_archive(deploy_dir: str) -> str:
    """Create deployment archive based on the operating system."""
    archive_name = "pong_windows" if platform.system() == "Windows" else "pong_linux"
    archive_ext = "zip" if platform.system() == "Windows" else "tar.gz"
    archive_path = os.path.join(os.getcwd(), f"{archive_name}.{archive_ext}")

    if os.path.exists(archive_path):
        os.remove(archive_path)

    if platform.system() == "Windows":
        with zipfile.ZipFile(archive_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
            for file in os.listdir(deploy_dir):
                file_path = os.path.join(deploy_dir, file)
                if os.path.isfile(file_path):
                    zipf.write(file_path, file)
    else:
        with tarfile.open(archive_path, "w:gz") as tar:
            for file in os.listdir(deploy_dir):
                file_path = os.path.join(deploy_dir, file)
                if os.path.isfile(file_path):
                    tar.add(file_path, arcname=file)

    return archive_path


def main():
    # Clean up previous build and deploy directories
    clean_directory(os.path.join(os.getcwd(), "deploy"), "deploy")
    clean_directory(os.path.join(os.getcwd(), "build"), "build")

    # Generate embedded resources first
    generate_embedded_resources()

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
    print("All resources are embedded in the executable - clean standalone deployment!")
    print("Archive contains only the executable file.")


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
