/********************************************************************************
 * Name:        waterjugpuzzle.ccp
 * Author:      Himmat Garcha
 * Description: Solves the water jug problem by using BFS
 * Pledge:      I pledge my honor that I have abided by the Stevens Honor System
 *******************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {

    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }

    // String representation of state in tuple form.
    string to_string() {

        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();

    }

};


// int capA,  capB,  and capC  are the jug capacities.
// int goalA, goalB, and goalC are the jug goals.
string bfs(int capA, int capB, int capC,
		   int goalA,int goalB,int goalC) {

	queue <State*> q;
	vector<vector<State*>> seen(capA+1, vector<State*>(capB+1));

	for (auto& row : seen) {
		for (auto& col : row) {
			col = nullptr;
		}
	}


	State* initial = new State(0, 0, capC, "Initial");
	State* current = new State(0, 0, capC, "Initial");


	q.push(current);

	while (!q.empty()) {

		current = q.front();
		q.pop();

		// Check if current equals goal_state
		if (current->a == goalA && current->b == goalB && current->c == goalC) {

			string result;

			State* copy = current;

			// Start back tracking current state's parents, adding to the output.
			while (current->directions != "Initial") {

				result.insert(0, current->directions);

				current=current->parent;

				if (current->directions != "Initial") {
					result.insert(0,"\n");
				}

			}

			current = copy;
			delete current;
			cout << "Initial state. " << initial->to_string() << endl;

			// popping queue q
			while (!(q.empty())) {

				delete q.front();
				q.pop();

			}

			delete initial;

			for (auto& row : seen) {
				for (auto& col : row) {
					delete col;
				}
			}

			return result;

		}

		// See if current has already been seen, otherwise add it to matrix
		if (seen[current->a][current->b] != nullptr) {

			delete current;
			continue;

		} else {
			seen[current->a][current->b] = current;
		}

		// Tries all 6 ways to pour water between the water jugs.

		// pour A into B
		if ((current->a != 0) && (current->b != capB)) {

			int remaining_space_in_b = capB - current->b;
			ostringstream directions;
			string temp = "temp";
			State* n_state = new State(current->a, current->b, current->c, temp);
			n_state->parent = current;

			if(remaining_space_in_b >= current->a) {

				n_state->b += n_state->a;
				n_state->a -= n_state->a;

				if (current->a == 1) {
					directions << "Pour " << current->a << " gallon from A to B. " << n_state->to_string();
				} else {
					directions << "Pour " << current->a << " gallons from A to B. " << n_state->to_string();
				}

			} else {

				n_state->b += remaining_space_in_b;
				n_state->a -= remaining_space_in_b;

				if (remaining_space_in_b == 1) {
					directions << "Pour " << remaining_space_in_b << " gallon from A to B. " << n_state->to_string();
				} else {
					directions << "Pour " << remaining_space_in_b << " gallons from A to B. " << n_state->to_string();
				}

			}

			string direct = directions.str();
			n_state->directions = direct;

			if (seen[n_state->a][n_state->b] != nullptr) {
				delete n_state;
			} else {
				q.push(n_state);
			}

		}

		// pour A into C
		if ((current->a != 0) && (current->c != capC)) {
			int remaining_space_in_c = capC - current->c;
			ostringstream directions;
			string temp = "temp";
			State* n_state = new State(current->a, current->b, current->c, temp);
			n_state->parent = current;

			if(remaining_space_in_c >= current->a) {
				n_state->c += n_state->a;
				n_state->a -= n_state->a;

				if (current->a == 1) {
					directions << "Pour " << current->a << " gallon from A to C. " << n_state->to_string();
				} else {
					directions << "Pour " << current->a << " gallons from A to C. " << n_state->to_string();
				}


			} else {
				n_state->c += remaining_space_in_c;
				n_state->a -= remaining_space_in_c;

				if (remaining_space_in_c == 1) {
					directions << "Pour " << remaining_space_in_c << " gallon from A to C. " << n_state->to_string();
				} else {
					directions << "Pour " << remaining_space_in_c << " gallons from A to C. " << n_state->to_string();
				}

			}

			string direct = directions.str();
			n_state->directions = direct;

			if (seen[n_state->a][n_state->b] != nullptr) {
				delete n_state;
			} else {
				q.push(n_state);
			}

		}

		// pour B into A
		if ((current->b != 0) && (current->a != capA)) {
			int remaining_space_in_a = capA - current->a;
			ostringstream directions;
			string temp = "temp";
			State* n_state = new State(current->a, current->b, current->c, temp);
			n_state->parent = current;

			if(remaining_space_in_a >= current->b) {
				n_state->a += n_state->b;
				n_state->b -= n_state->b;

				if (current->b == 1) {
					directions << "Pour " << current->b << " gallon from B to A. " << n_state->to_string();
				} else {
					directions << "Pour " << current->b << " gallons from B to A. " << n_state->to_string();
				}

			} else {
				n_state->a += remaining_space_in_a;
				n_state->b -= remaining_space_in_a;

				if (remaining_space_in_a == 1) {
					directions << "Pour " << remaining_space_in_a << " gallon from B to A. " << n_state->to_string();
				} else {
					directions << "Pour " << remaining_space_in_a << " gallons from B to A. " << n_state->to_string();
				}


			}

			string direct = directions.str();
			n_state->directions = direct;

			if (seen[n_state->a][n_state->b] != nullptr) {
				delete n_state;
			} else {
				q.push(n_state);
			}

		}

		// pour B into C
		if ((current->b != 0) && (current->c != capC)) {
			int remaining_space_in_c = capC - current->c;
			ostringstream directions;
			string temp = "temp";
			State* n_state = new State(current->a, current->b, current->c, temp);
			n_state->parent = current;

			if(remaining_space_in_c >= current->b) {
				n_state->c += n_state->b;
				n_state->b -= n_state->b;

				if (current->b == 1) {
					directions << "Pour " << current->b << " gallon from B to C. " << n_state->to_string();
				} else {
					directions << "Pour " << current->b << " gallons from B to C. " << n_state->to_string();
				}

			} else {
				n_state->c += remaining_space_in_c;
				n_state->b -= remaining_space_in_c;

				if (remaining_space_in_c == 1) {
					directions << "Pour " << remaining_space_in_c << " gallon from B to C. " << n_state->to_string();
				} else {
					directions << "Pour " << remaining_space_in_c << " gallons from B to C. " << n_state->to_string();
				}

			}

			string direct = directions.str();
			n_state->directions = direct;

			if (seen[n_state->a][n_state->b] != nullptr) {
				delete n_state;
			} else {
				q.push(n_state);
			}

		}

		// pour C into A
		if ((current->c != 0) && (current->a != capA)) {
			int remaining_space_in_a = capA - current->a;
			ostringstream directions;
			string temp = "temp";
			State* n_state = new State(current->a, current->b, current->c, temp);
			n_state->parent = current;

			if(remaining_space_in_a >= current->c) {
				n_state->a += n_state->c;
				n_state->c -= n_state->c;

				if (current->c == 1) {
					directions << "Pour " << current->c << " gallon from C to A. " << n_state->to_string();
				} else {
					directions << "Pour " << current->c << " gallons from C to A. " << n_state->to_string();
				}

			} else {
				n_state->a += remaining_space_in_a;
				n_state->c -= remaining_space_in_a;

				if (remaining_space_in_a == 1) {
					directions << "Pour " << remaining_space_in_a << " gallon from C to A. " << n_state->to_string();
				} else {
					directions << "Pour " << remaining_space_in_a << " gallons from C to A. " << n_state->to_string();
				}

			}

			string direct = directions.str();
			n_state->directions = direct;

			if (seen[n_state->a][n_state->b] != nullptr) {
				delete n_state;
			} else {
				q.push(n_state);
			}

		}

		// pour C into B
		if ((current->c != 0) && (current->b != capB)) {
			int remaining_space_in_b = capB - current->b;
			ostringstream directions;
			string temp = "temp";
			State* n_state = new State(current->a, current->b, current->c, temp);
			n_state->parent = current;

			if(remaining_space_in_b >= current->c) {
				n_state->b += n_state->c;
				n_state->c -= n_state->c;

				if (current->b == 1) {
					directions << "Pour " << current->b << " gallon from C to B. " << n_state->to_string();
				} else {
					directions << "Pour " << current->b << " gallons from C to B. " << n_state->to_string();
				}

			} else {
				n_state->b += remaining_space_in_b;
				n_state->c -= remaining_space_in_b;

				if (remaining_space_in_b == 1) {
					directions << "Pour " << remaining_space_in_b << " gallon from C to B. " << n_state->to_string();
				} else {
					directions << "Pour " << remaining_space_in_b << " gallons from C to B. " << n_state->to_string();
				}
			}

			string direct = directions.str();
			n_state->directions = direct;

			if (seen[n_state->a][n_state->b] != nullptr) {
				delete n_state;
			} else {
				q.push(n_state);
			}
		}

	}


	delete initial;

	for (auto& row : seen) {
		for (auto& col : row) {
			delete col;
		}
	}

	return "No solution.";

}


int main(int argc, char * const argv[]) {
	int CA, CB, CC, GA, GB, GC;
	istringstream iss;

	if (argc != 7) {

		cerr << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
		return 1;

	}

	// Checks if input is acceptable for the capacities.

	// Checks A capacity
	iss.str(argv[1]);
	if (!(iss >> CA) || CA < 0) {

		cerr << "Error: Invalid capacity '" << argv[1] << "' for jug A." << endl;
		return 1;

	}
	iss.clear();

	// Checks B capacity
	iss.str(argv[2]);
	if (!(iss >> CB) || CB < 0) {

		cerr << "Error: Invalid capacity '" << argv[2] << "' for jug B." << endl;
		return 1;

	}
	iss.clear();

	// Checks C capacity
	iss.str(argv[3]);
	if (!(iss >> CC) || CC <= 0) {

		cerr << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
		return 1;

	}
	iss.clear();


	// Checks if input is acceptable for the goals.

	// Checks A goal
	iss.str(argv[4]);
	if (!(iss >> GA) || GA < 0) {

		cerr << "Error: Invalid goal '" << argv[4] << "' for jug A." << endl;
		return 1;

	}
	iss.clear();

	// Checks B goal
	iss.str(argv[5]);
	if (!(iss >> GB) || GB < 0) {

		cerr << "Error: Invalid goal '" << argv[5] << "' for jug B." << endl;
		return 1;

	}
	iss.clear();

	// Checks C goal
	iss.str(argv[6]);
	if (!(iss >> GC) || GC < 0) {

		cerr << "Error: Invalid goal '" << argv[6] << "' for jug C." << endl;
		return 1;

	}
	iss.clear();


	// Checks to see if goals exceed capacities.

	if (CA < GA) {

		cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
		return 1;

	}

	if (CB < GB) {

		cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
		return 1;

	}

	if (CC < GC) {

		cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
		return 1;

	}

	if (CC != (GA + GB + GC)) {

		cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
		return 1;

	}

	string answer = bfs(CA, CB, CC, GA, GB, GC);
	cout << answer << endl;

    return 0;

}