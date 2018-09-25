import os
from conan.packager import ConanMultiPackager
import time


if __name__ == "__main__":
    # ConanPackageTools
    # See: https://github.com/conan-io/conan-package-tools/blob/develop/README.md
    builder = ConanMultiPackager()
    builder.add_common_builds()

    # Add environment variables to build definitions
    XMS_VERSION = os.environ.get('XMS_VERSION', None)
    python_target_version = os.environ.get('PYTHON_TARGET_VERSION', "3.6")

    for settings, options, env_vars, build_requires, reference in builder.items:
        # General Options
        env_vars.update({
            'XMS_VERSION': XMS_VERSION,
            'VERBOSE': 1,
        })

        # Require c++11 compatibility
        if settings['compiler'] == 'gcc':
            settings.update({
                'compiler.libcxx': 'libstdc++11'
            })

    updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # pybind option
        if settings['compiler'] == "Visual Studio" \
          and settings['compiler.version'] == "12") \
            updated_builds.append([settings, options, env_vars, build_requires])

        updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = updated_builds

    # print(builder.builds)
    builder.run()
