This repository contains the cleaned up verion of the slt core libraries.

## Caveats
On paper, this is meant to be platform agnostic, but in practice, the CMake
configuration has only been tested on windows for the time being

# Contributor's guide
## Code organization

- Source files, tests and internal headers go in the src/slt directory
- All headers meant to be copied into the toolset's release belong in the 
  include/slt directory
- Headers in the include/slt directory are meant to be read as documentation
- If modules need to expose implementations or declare types/functions that
  are not relevant to a user of the toolset, these declarations/definitions
  must go in the module/details directory, and may not share the same namespace
  as user-facing constructs.
- Avoid having headers with identical file names within the same module, it 
  confuses the MSVC project.
