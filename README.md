# 🚀 Optimal Path for Data Transfer

A C-based implementation of graph algorithms for efficient network routing and data transfer optimization. This project compares multiple graph representations and algorithms to determine the optimal communication path between network nodes based on shortest path and maximum bandwidth.

---

## 📖 Overview

Modern computer networks require efficient routing techniques to minimize latency and maximize bandwidth. This project models a communication network as a weighted graph and implements multiple graph data structures to analyze routing performance.

The project supports:

- Shortest path computation
- Maximum bandwidth path computation
- Multiple graph representations
- CSV-based network topology input
- Performance comparison of different implementations

---

## ✨ Features

- 📌 Edge List Graph
- 📌 Adjacency List Graph
- 📌 Adjacency Matrix Graph
- 📌 Dijkstra's Algorithm (Shortest Path)
- 📌 Edmonds-Karp Algorithm (Maximum Bandwidth Path)
- 📌 IP Address to Vertex Mapping
- 📌 CSV-based Network Input
- 📌 Experimental Performance Evaluation

---

## 🛠️ Technologies Used

| Category | Technology |
|-----------|------------|
| Language | C |
| Concepts | Data Structures & Algorithms |
| Graph Representations | Edge List, Adjacency List, Adjacency Matrix |
| Algorithms | Dijkstra, Edmonds-Karp |
| Input Format | CSV |
| Development Environment | Visual Studio Code |

---

## 📂 Project Structure

```
Optimal-Path-For-Data-Transfer/
│
├── include/                # Header files
│
├── src/                    # Source code
│
├── test_cases/             # Input CSV test cases
│
├── results/                # Output files and reports
│
├── README.md
├── LICENSE
└── .gitignore
```

---

## ⚙️ Algorithms Implemented

### 🔹 Shortest Path

Uses **Dijkstra's Algorithm** to compute the minimum-cost route between two nodes in the network.

Implemented using:

- Edge List
- Adjacency List
- Adjacency Matrix

---

### 🔹 Maximum Bandwidth Path

Uses the **Edmonds-Karp Algorithm** to determine the path capable of carrying the highest bandwidth between source and destination.

Implemented using:

- Edge List
- Adjacency List
- Adjacency Matrix

---

## 📥 Input

Network topology is provided as CSV files containing:

- Source IP
- Destination IP
- Edge Weight (Cost/Bandwidth)

Example:

```
SourceIP,DestinationIP,Weight
192.168.1.1,192.168.1.2,10
192.168.1.2,192.168.1.3,15
```

---

## ▶️ Running the Project

### Clone the repository

```bash
git clone https://github.com/your-username/Optimal-Path-For-Data-Transfer.git
```

### Navigate to the project

```bash
cd Optimal-Path-For-Data-Transfer
```

### Compile

Example using GCC:

```bash
gcc src/*.c -Iinclude -o optimal_path
```

### Run

```bash
./optimal_path
```

---

## 📊 Performance Analysis

The project compares the execution efficiency of:

- Edge List
- Adjacency List
- Adjacency Matrix

Evaluation metrics include:

- Execution Time
- Memory Usage
- Routing Efficiency
- Algorithm Performance

Experimental outputs are available in the **results/** directory.

---

## 📁 Test Cases

Sample datasets are available inside the **test_cases/** folder.

These datasets can be modified or extended to evaluate the algorithms under different network conditions.

---

## 🎯 Learning Outcomes

This project demonstrates practical implementation of:

- Graph Data Structures
- Shortest Path Algorithms
- Network Routing
- Maximum Flow Algorithms
- File Handling in C
- Performance Evaluation of Algorithms

---

## 🔮 Future Enhancements

- Bellman-Ford Algorithm
- Floyd-Warshall Algorithm
- Dynamic Network Updates
- GUI Visualization
- Real-time Network Simulation
- Parallel Graph Processing

---

## 👩‍💻 Authors

**Bommisetty Nikhita**

Amrita Vishwa Vidyapeetham

Bachelor of Technology in Computer Science (Artificial Intelligence)

---

## 📄 License

This project is licensed under the MIT License.

---

## ⭐ Support

If you found this project useful, consider giving the repository a ⭐ on GitHub.