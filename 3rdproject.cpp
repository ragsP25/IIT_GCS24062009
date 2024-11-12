#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
int id;
int arrivalTime;
int coresRequired;
int memoryRequired;
int executionTime;

int grossValue() const {
return executionTime * coresRequired * memoryRequired;
}
};

struct WorkerNode {
int id;
int availableCores;
int availableMemory;

WorkerNode(int id) : id(id), availableCores(24), availableMemory(64) {}
};

class JobScheduler {
private:
vector<Job> jobQueue;
vector<WorkerNode> workerNodes;
string queuePolicy;
string fitPolicy;

public:
JobScheduler(int workerCount, string queuePolicy, string fitPolicy) 
: queuePolicy(queuePolicy), fitPolicy(fitPolicy) {
for (int i = 0; i < workerCount; ++i) {
workerNodes.push_back(WorkerNode(i));
}
}

void addJob(Job job) {
jobQueue.push_back(job);
}

void applyQueuePolicy() {
if (queuePolicy == "FCFS") {
sort(jobQueue.begin(), jobQueue.end(), [](Job a, Job b) {
return a.arrivalTime < b.arrivalTime;
});
} else if (queuePolicy == "SmallestJob") {
sort(jobQueue.begin(), jobQueue.end(), [](Job a, Job b) {
return a.grossValue() < b.grossValue();
});
} else if (queuePolicy == "ShortDuration") {
sort(jobQueue.begin(), jobQueue.end(), [](Job a, Job b) {
return a.executionTime < b.executionTime;
});
}
}

int findWorkerNode(Job job) {
for (auto& node : workerNodes) {
if (node.availableCores >= job.coresRequired && node.availableMemory >= job.memoryRequired) {
return node.id;
}
}
return -1;
}

void executeScheduler() {
applyQueuePolicy();
for (auto& job : jobQueue) {
int nodeID = findWorkerNode(job);
if (nodeID != -1) {
workerNodes[nodeID].availableCores -= job.coresRequired;
workerNodes[nodeID].availableMemory -= job.memoryRequired;
cout << "Job " << job.id << " allocated to Worker " << nodeID << "\n";
} else {
cout << "Job " << job.id << " could not be allocated, reinserted in queue.\n";
}
}
}
};

int main() {
JobScheduler scheduler(128, "FCFS", "FirstFit");

scheduler.addJob({1, 0, 4, 8, 2});
scheduler.addJob({2, 1, 2, 16, 1});
scheduler.addJob({3, 2, 8, 32, 4});

scheduler.executeScheduler();

return 0;
}
