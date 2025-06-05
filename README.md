<table>
<tr style="border: none">
<td style="border: none">


# Algorithm Overview

Gather all legal moves (src→dest) for the side to move.

Split that move list into equal “chunks,” one per thread.

Each thread evaluates its chunk with evaluateMove(…), maintaining a small local priority queue of its best candidates.

When a thread finishes, it merges its top candidates into a single global priority queue under a short‐lived mutex lock.

Finally, we extract the top‐N moves (by descending score) from the global queue.

# THE Chess Template Repository

</td>
<td align="right" style="border: none">
<img src="./img/scaleup.png" alt="Scaleup" height="100">
</td>
</tr>

</table>
This is a template repository for the Tel Hai Excellenteam (THE) CPP course.

All exercises and submissions should follow the format of this repository. For your convenience, you can start each assignment by cloning this template.

## Environment Setup
During the course we will utilize Linux based operating system (OS), to run and execute programs.

### Windows Installation
1. Install [WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install
) , with Ubuntu distribution.


### Mac Installation
1. Follow the instruction presented in the following [video]( https://www.youtube.com/watch?v=LjL_N0OZxvY
), install Ubuntu (no GUI) version
2. To install GUI, use following [guide](https://askubuntu.com/questions/53822/how-do-you-run-ubuntu-server-with-a-gui
)
3. In case you forgot the default credentials, read following [article](https://www.debugpoint.com/virtualbox-id-password/
).


### Linux Installation
Ensure you have `g++`, `CMake`, and `make` installed:

```sh
sudo apt update
sudo apt install -y g++ cmake make
```

## Grading and Conventions
Your assignment will be graded according to the following criteria. Please make sure your assignment follows the standards and conventions outlined below:


For best practices please use examples presented in [PRACTICES.md](PRACTICES.md), if still there is an uncertainty or missing use cases, you are highly encouraged to contact the staff for further clarifications.

### Branching
Use the [CONTRIBUTING.md](CONTRIBUTING.md) file as your guideline for proper use of Git. For more information, you are encouraged to search online for "[GitHub Workflow Methodology](https://www.youtube.com/watch?v=U_IFGpJDbeU&ab_channel=DevOpsToolkit)."


### Repository Structure
Please follow the guidelines in this section strictly.

- The repository should include a `CMakeLists.txt` file at the root directory.
- The repository **MUST NOT** include any compiled binaries (e.g., build/, .o files, a.out, or any other generated executables).(put those file names in `.gitignore` file)

<p align="center">
  <img src="./img/cpp-logo.png" alt="C++ Logo" width="100" height="100">
  <img src="./img/linux-logo.jpg" alt="Linux Logo" width="100" height="100">
</p>    

#### GitHub Configuration
* The repository should include a `README.md` file at the root directory.
* The repository should include a `CONTRIBUTING.md` file at the root directory.
* You must have a `.gitignore` file, and there shouldn’t be any unnecessary files in the repository.
* 🚨 **CRITICAL:** Repositories without a workflow file at `.github/workflows/c-cpp.yml` will not be graded. :(
* You must have an `img` directory at the root directory.

#### Project's Files

- The repository should include a `main.cpp` file in src directory.
- The repository should include a `src` directory at the root.
  - All source files should be placed in the `src` directory.
- The repository should include a `include` directory at the root.
  - All `.h` files should be placed in the `include` directory.
- The repository should include a `test` directory at the root.
  - All tests should be placed in the `test` directory.


#### Project Tree

Project tree should match the following structure:

```bash
.
├── CONTRIBUTING.md
├── .git
│   ├── ...
├── .github
│   └── workflows
│       └── c-cpp.yml
├── .gitignore
├── img
│   ├── excellenteam.png
│   ├── scaleup.png
│   └── cpp_logo.png
├── README.md
├── CMakeLists.txt
├── src
│   ├── main.cpp
│   ├── example.cpp
│   ├── ...
├── include
│   ├── example.h
│   ├── ...
├── tests
│   ├── ...
```
## Compilation Instructions
The evaluators will compile your code using **CMake**. Ensure your project follows the CMake structure correctly.

To compile your project manually, use the following commands:

```sh
mkdir -p build
cd build
cmake ..
make
./Chess 
```

Any project that does not compile properly using these steps will not be graded.


## How to Submit an Exercise
You are required to submit each exercise using "GitHub Classroom". To do this, you must upload a link to your "GitHub Classroom" repository via Moodle..

### Branching and Pull Request Guidelines

- 🚨 Your `main` branch **must remain clean and stable** at all times. Never push directly to `main`.  
- Create a **new branch** from `main` for each exercise (e.g., `exercise1`, `feature/queen-movement`, etc.).  
- Work on those branches and push your changes there.  
- Once you're done, open a **Pull Request (PR)** from your branch where all the changes where made into `main`.  
- In the PR, review your changes carefully using GitHub’s diff viewer.  
- Make sure all checks pass (e.g., linter, build).  
- After merging the PR into `main`, you can submit the repository link via Moodle.


### Pre-submission Checkup
1. Make sure you've answered all the questions.  
2. Review and refactor your code for better readability (ideally, review your code one or two days later — sometimes it's better to review with fresh eyes).  
3. Ensure that all intended files are uploaded to Git and follow the structure convention outlined in the [Repository Structure](#repository-structure) section.  
4. Ensure that your code is running.  
5. Once you open a PR, review the changes **carefully**. You can leverage GitHub's built-in diff viewer.  
6. Wait and confirm that the linter test completed successfully. If the linter test fails, assess the errors and refactor accordingly — otherwise, each error will negatively impact your grade.  
7. Make sure you followed the [Branching and Pull Request Guidelines](#branching-and-pull-request-guidelines).  
8. Upload the repository link to Moodle.  
9. Good luck :)

<!-- Center Excellenteam image -->
<p align="center">
  <img src="./img/excellenteam.png" alt="Excellenteam">
</p>
