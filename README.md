# 🐰 Easter Bunny IPC Event

A C program simulating a fun and competitive **Easter Bunny event** using **Inter-Process Communication (IPC)** mechanisms on Linux.  
Bunny contestants register with their names and poems, then compete to become the **Easter Bunny King** by collecting the most red eggs!

## 📜 Features

- Bunny registration with name and poem
- Persistent storage in `bunnies.txt`
- Modifying and deleting registered bunnies
- Determining the Easter Bunny King (most eggs)
- **Child processes** (bunny boys) recite poems and collect eggs
- **Signals** (`SIGUSR1`) used for notifying the Chief Bunny
- **Pipe** used for sending egg counts from children to parent
- Temporary file (`temp.txt`) ensures safe file updates

## 🧠 Technologies & Concepts Used

- C Programming
- Inter-Process Communication:
  - `fork()`
  - `pipe()`
  - `signal()`, `kill()`, `SIGUSR1`
  - `wait()`
- File I/O
- Basic buffer protection with `strncpy`

## 🗂 Menu Options
	1.	Register a new bunny
	2.	List all bunnies
	3.	Modify a bunny
	4.	Delete a bunny
	5.	Show Easter Bunny King
	6.	Start Watering (forks child processes)
	7.	Exit
 ## 🐣 Start Watering Explanation

- Parent creates a pipe.
- For each registered bunny:
  - A child process is forked.
  - The child sends a signal to the parent using `kill(getppid(), SIGUSR1)`.
  - Child prints its poem and sends a random egg count (1–20) through the pipe.
- Parent:
  - Waits for each child with `wait(NULL)`
  - Reads egg counts from the pipe
  - Updates `bunnies.txt` safely via a temp file

## 📌 Sample Output
Chief Bunny received a signal from a bunny.
Fluffy Whiskers recites: “Springtime blooms and eggs so red…”
pinky recites: “Roses are red, violets are blue…”
…
Easter Bunny King
Name: Fluffy Whiskers
Poem: Springtime blooms and eggs so red…
Eggs: 20

## 🛡 Safety & Considerations

- Uses `strncpy()` to limit buffer overflows
- Uses `srand(getpid())` in child to ensure different random seeds
- Uses `pipe()` in correct direction (child → parent)
- Cleans up files properly (safe rename of `temp.txt`)

## 📎 Compile & Run

```bash
gcc easter_bunny.c -o easter_bunny.out
./easter_bunny.out
