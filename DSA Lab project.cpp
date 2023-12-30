#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <unordered_map>

using namespace std;

// Defining a structure to represent a candidate
struct Candidate {
    string name;
    int votes;

    Candidate(const string& n) : name(n), votes(0) {}
};

// Binary Search Tree Node
struct TreeNode {
    Candidate* candidate;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Candidate* c) : candidate(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree Class
class VotingSystem {
private:
    queue<pair<int, Candidate*>> voteQueue;  // Pairing voter ID with candidate

    unordered_map<int, Candidate*> votedCandidates;  // Map to keep track of voted candidates

    // Insert a candidate into the BST
    TreeNode* insert(TreeNode* node, Candidate* candidate) {
        if (node == nullptr) {
            return new TreeNode(candidate);
        }

        if (candidate->name < node->candidate->name) {
            node->left = insert(node->left, candidate);
        } else if (candidate->name > node->candidate->name) {
            node->right = insert(node->right, candidate);
        }

        return node;
    }

    // Inorder traversal to display candidates and votes
    void displayInOrder(TreeNode* node) {
        if (node != nullptr) {
            displayInOrder(node->left);
            cout << setw(20) << left << node->candidate->name << " : " << node->candidate->votes << " votes\n";
            displayInOrder(node->right);
        }
    }

public:
    VotingSystem() : root(nullptr) {}

    // Add a candidate to the system
    void addCandidate(const string& name) {
        Candidate* candidate = new Candidate(name);
        root = insert(root, candidate);
        cout << "Candidate '" << name << "' added successfully.\n";
    }

    // Register a voter in the system
    void registerVoter(int voterID, Candidate* candidate) {
        voteQueue.push({voterID, candidate});
        cout << "Voter with ID " << voterID << " is now waiting in line for candidate '" << candidate->name << "'.\n";
    }

    // Vote for a candidate
    void vote(int voterID) {
        if (!voteQueue.empty()) {
            pair<int, Candidate*> voter = voteQueue.front();
            if (voter.first == voterID) {
                voteQueue.pop();
                Candidate* candidate = voter.second;

                TreeNode* node = findCandidate(root, candidate->name);
                if (node != nullptr) {
                    node->candidate->votes++;
                    votedCandidates[voterID] = candidate;
                    cout << "Vote for candidate '" << candidate->name << "' recorded by voter with ID " << voterID << ".\n";
                } else {
                    cout << "Candidate '" << candidate->name << "' not found.\n";
                }
            } else {
                cout << "It's not the turn for voter with ID " << voterID << ".\n";
            }
        } else {
            cout << "No candidates in the voting queue.\n";
        }
    }

    // Display the list of candidates and votes
    void displayCandidates() {
        cout << "\nCandidates and Votes:\n";
        displayInOrder(root);
    }

    // Display candidates waiting in line
    void displayQueue() {
        cout << "\nCandidates Waiting in Line:\n";
        queue<pair<int, Candidate*>> tempQueue = voteQueue;
        while (!tempQueue.empty()) {
            cout << "Voter ID: " << tempQueue.front().first << " for candidate '" << tempQueue.front().second->name << "'\n";
            tempQueue.pop();
        }
    }

    // Display voted candidates
    void displayVotedCandidates() {
        cout << "\nVoted Candidates:\n";
        for (const auto& entry : votedCandidates) {
            cout << "Voter ID: " << entry.first << " voted for candidate '" << entry.second->name << "'\n";
        }
    }

    // Helper function to find a candidate in the BST
    TreeNode* findCandidate(TreeNode* node, const string& name) {
        while (node != nullptr) {
            if (name < node->candidate->name) {
                node = node->left;
            } else if (name > node->candidate->name) {
                node = node->right;
            } else {
                return node;
            }
        }
        return nullptr;
    }

    // Sort candidates by votes (descending order)
    void sortCandidatesByVotes() {
        vector<Candidate*> candidates;
        collectCandidates(root, candidates);

        sort(candidates.begin(), candidates.end(), [](Candidate* a, Candidate* b) {
            return a->votes > b->votes;
        });

        cout << "\nCandidates sorted by Votes (Descending Order):\n";
        for (const auto& candidate : candidates) {
            cout << setw(20) << left << candidate->name << " : " << candidate->votes << " votes\n";
        }
    }

    // Helper function to collect candidates in vector for sorting
    void collectCandidates(TreeNode* node, vector<Candidate*>& candidates) {
        if (node != nullptr) {
            collectCandidates(node->left, candidates);
            candidates.push_back(node->candidate);
            collectCandidates(node->right, candidates);
        }
    }

    TreeNode* root;
};

// Display an attractive menu
void displayMenu() {
    cout << "==== Online Voting System ====\n";
    cout << "1. Add Candidate\n";
    cout << "2. Register Voter\n";
    cout << "3. Vote\n";
    cout << "4. Display Candidates\n";
    cout << "5. Display Queue\n";
    cout << "6. Display Voted Candidates\n";
    cout << "7. Sort Candidates by Votes\n";
    cout << "8. Exit\n";
}

int main() {
    VotingSystem votingSystem;
    int voterIDCounter = 1;

    while (true) {
        displayMenu();

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                // Add Candidate
                string candidateName;
                cout << "Enter candidate name: ";
                cin.ignore(); // Ignore newline character from previous input
                getline(cin, candidateName);
                votingSystem.addCandidate(candidateName);
                break;
            }
            case 2: {
                // Register Voter
                int voterID;
                string candidateName;
                cout << "Enter your voter ID: ";
                cin >> voterID;

                cout << "Enter the candidate you want to vote for: ";
                cin.ignore(); // Ignore newline character from previous input
                getline(cin, candidateName);

                Candidate* candidate = votingSystem.findCandidate(votingSystem.root, candidateName)->candidate;
                if (candidate != nullptr) {
                    votingSystem.registerVoter(voterID, candidate);
                    voterIDCounter = max(voterIDCounter, voterID + 1);
                } else {
                    cout << "Candidate '" << candidateName << "' not found.\n";
                }
                break;
            }
            case 3: {
                // Vote
                int voterID;
                cout << "Enter your voter ID: ";
                cin >> voterID;
                votingSystem.vote(voterID);
                break;
            }
            case 4:
                // Display Candidates
                votingSystem.displayCandidates();
                break;
            case 5:
                // Display Queue
                votingSystem.displayQueue();
                break;
            case 6:
                // Display Voted Candidates
                votingSystem.displayVotedCandidates();
                break;
            case 7:
                // Sort Candidates by Votes
                votingSystem.sortCandidatesByVotes();
                break;
            case 8:
                // Exit
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
