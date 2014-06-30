/**
 * \class	SimplexTree
 * \brief	Stores a bifiltered simplicial complex in a simplex tree structure.
 * \author	Matthew L. Wright
 * \date	February 2014
 * 
 * The SimplexTree class stores a bifiltered simplicial complex in a simplex tree structure.
 * Each node in the simplex tree (implemented by the STNode class) represents one simplex in the bifiltration.
 * Each simplex has a multi-index at which it is born.
 * Implementation is based on a 2012 paper by Boissonnat and Maria.
 */


#ifndef __SimplexTree_H__
#define __SimplexTree_H__

#include <utility>	// std::pair
#include <map>
#include "point.h"
#include "st_node.h"
#include "map_matrix.h"

struct SimplexData;	//used for return type of SimplexTree::get_simplex_data()

class SimplexTree {
	public:
		SimplexTree(int v);							//constructor
		
		void build_VR_complex(std::vector<Point> &, int, int, double);		//builds SimplexTree representing a Vietoris-Rips complex from a vector of points, with certain parameters
		
		void add_simplex(std::vector<int> & vertices, int time, int dist);	//adds a simplex (and its faces) to the SimplexTree
		
		void update_global_indexes();			//updates the global indexes of all simplices in this simplex tree
		
		int dist_index(double);				//returns the index of a distance value, or -1 if not found
		double get_dist(int);				//returns a distance value, given an index
		
		int time_index(double);				//returns the index of a time value, or -1 if not found
		double get_time(int);				//returns a time value, given an index
		
		MapMatrix* get_boundary_mx(int time, int dist, int dim);	//computes a boundary matrix for simplices of a given dimension at the specified multi-index
		MapMatrix* get_merge_mx(int time, int dist, int dim);		//computes a matrix representing the map [B+C,D], for inclusion maps into the dim-skeleton at the specified multi-index
		MapMatrix* get_split_mx(int time, int dist, int dim);		//computes a matrix representing the map [A,B+C], for the dim-skeleton at the specified multi-index
		
		MapMatrix* get_boundary_mx(std::vector<int> coface_global, std::map<int,int> face_order);	//computes a boundary matrix, using given orders on simplices of dimensions d (cofaces) and d-1 (faces)
		
		std::vector<int> find_vertices(int);		//given a global index, return (a vector containing) the vertices of the simplex
		int find_index(std::vector<int>&);		//given a sorted vector of vertex indexes, return the global index of the corresponding simplex (or -1 if it doesn't exist)
		
		SimplexData get_simplex_data(int index);	//returns the (time, dist) multi-index of the simplex with given global simplex index, as well as the dimension of the simplex
		
		int get_num_dists();		//returns the number of unique distance indexes
		int get_num_times();		//returns the number of unique time indexes
		int get_num_simplices();		//returns the total number of simplices represented in the simplex tree
			//TODO: would it be more efficient to store the total number of simplices???
		
		void print();				//prints a representation of the simplex tree
		
        void test_lists();  //TESTING ONLY
		
	private:
		std::vector<double> dist_list;	//sorted list of unique distances (not greater than max_distance), used for creating integer indexes
		std::vector<double> time_list;	//sorted list of unique birth times, used for creating integer indexes
		STNode root;		//root node of the simplex tree
		
		const int verbosity;	//controls display of output, for debugging
		
		void build_VR_subtree(std::vector<Point> &points, double* distances, STNode &parent, std::vector<int> &parent_indexes, double prev_time, double prev_dist, int current_depth, int max_depth, int& gic);	//recursive function used in build_VR_complex()
		
		void add_faces(std::vector<int> & vertices, int time, int dist);	//recursively adds faces of a simplex to the SimplexTree; WARNING: doesn't update global data structures (time_list, dist_list, or global indexes), so should only be called from add_simplex()
		
		void update_gi_recursively(STNode &node, int &gic); 		//recursively update global indexes of simplices
		
		void find_nodes(STNode &node, int level, std::vector<int> &vec, int time, int dist, int dim);	//recursively search tree for simplices of specified dimension that exist at specified multi-index
		
		void find_vertices_recursively(std::vector<int> &vertices, STNode &node, int key);	//recursively search for a global index and keep track of vertices

		void print_subtree(STNode&, int);	//recursive function that prints the simplex tree
};

#include "simplex_tree.cpp"

#endif // __SimplexTree_H__