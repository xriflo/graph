#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <limits>
#include <list>


using namespace std;
class Vertex;
class Neighbour;


class Vertex {
public:
	string name;
	Neighbour* neighbours;
	Vertex* next;
	Vertex(string const& vertix_name) : 
		name(vertix_name), 
		neighbours(nullptr), 
		next(nullptr){}
	Vertex(){}
	bool operator==(const Vertex& other) const;
  	bool operator!=(const Vertex& other) const;
};

class Neighbour {
public:
	Vertex neighbour;
	unsigned int distance;
	Neighbour* next;
	Neighbour(Vertex neighbourx, unsigned int distancex) : 
		neighbour(neighbourx), 
		distance(distancex), 
		next(nullptr) {}
};

class Graph {
public:
	Vertex* vertices;
	void init_graph() {vertices = nullptr;}
	void init_graph(string filename);
	void add_vertex(string name);
	void add_edge(string const& v1, string const& v2, unsigned int distance);
	bool contains_vertex(string const& v);
	bool contains_edge(string const& v1, string const& v2);
	unsigned int get_distance(string const& v1, string const& v2);
	void print_graph();
	Vertex* getVertex(string const& name);
	unsigned int apply_dijkstra(string const& source, string const& target);
};


bool Vertex::operator==(const Vertex& other) const {
	return name==other.name;
}
bool Vertex::operator!=(const Vertex& other) const {
return (name!=other.name);
}

namespace std
{
    template <>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const & v) const noexcept
        {
            return std::hash<std::string>()(v.name);
        }
    };
};

ostream& operator<<(ostream& os, const Vertex& v)  
{  
	os << v.name;
    return os;  
}  

void Graph::init_graph(string filename) {
	init_graph();
	string in, out;
	unsigned int dist;
	std::ifstream infile(filename);
	while(infile>>in>>out>>dist) {
		add_vertex(in);
		add_vertex(out);
		add_edge(in, out, dist);
	}
}

void Graph::add_vertex(string name) {
	Vertex* vertex = new Vertex(name);
	//if the list is empty make the list to point to the new element
	if(vertices==nullptr) {
		vertices = vertex;
	}
	//else add the element to the end of the list
	else {
		Vertex* p = vertices;
		bool exists = false;
		//check the vertex if already exists
		while(p!=nullptr) {
			if(*p==*vertex)
				exists = true;
			p = p->next;
		}
		//it it does not exists, add it
		if(not exists) {
			p = vertices;
			while(p->next!=nullptr)
				p = p->next;
			p->next = vertex;
		}
	}
}

void Graph::add_edge(string const& v1, string const& v2, unsigned int distance) {
	Vertex *start, *end;
	Vertex* p = vertices;
	start = getVertex(v1);
	end = getVertex(v2);

	if(start!=nullptr && end!=nullptr) {
		Neighbour* neighbour = new Neighbour(*end, distance);
		if(start->neighbours==nullptr) {
			start->neighbours = neighbour;
		}
		else {
			Neighbour* curr_neighbour = start->neighbours;
			while(curr_neighbour->next!=nullptr) {
				curr_neighbour = curr_neighbour->next;
			}
			curr_neighbour->next = neighbour;
		}
	}
}

bool Graph::contains_vertex(string const& v) {
	Vertex* p = vertices;
	while(p!=nullptr) {
		if(v==p->name)
			return true;
		p = p->next;
	}
	return false;
}

bool Graph::contains_edge(string const& v1, string const& v2) {
	if(v1==v2)
		return true;
	Vertex *start=nullptr, *end=nullptr;
	Vertex* p = vertices;
	start = getVertex(v1);

	if(start!=nullptr) {
		Neighbour* n = start->neighbours;
		while(n!=nullptr) {
			if(v2==n->neighbour.name) {
				end = &n->neighbour;
				break;
			}
			n = n->next;
		}
		if(end!=nullptr)
			return true;
	}

	return false;
}

unsigned int Graph::get_distance(string const& v1, string const& v2) {
	if(v1==v2)
		return 0;
	Vertex *start=nullptr, *end=nullptr;
	Vertex* p = vertices;
	start = getVertex(v1);

	if(start!=nullptr) {
		Neighbour* n = start->neighbours;
		while(n!=nullptr) {
			if(v2==n->neighbour.name) {
				return n->distance;
			}
			n = n->next;
		}
	}
	return -1;
}

void Graph::print_graph() {
	Vertex* p = vertices;
	while(p!=nullptr) {
		cout<<p->name<<": ";
		Neighbour* n = p->neighbours;
		while(n!=nullptr) {
			cout<<n->neighbour.name<<"("<<n->distance<<")"<<" ";
			n=n->next;
		}
		p = p->next;
		cout<<endl;
	}
}

Vertex* Graph::getVertex(string const& name) {
	Vertex* p = vertices;
	while(p!=nullptr) {
		if(name==p->name) {
			return p;
		}
		p = p->next;
	}
	return nullptr;
}

unsigned int Graph::apply_dijkstra(string const& source, string const& target) {
	Vertex* start = getVertex(source);
	Vertex* end = getVertex(target);
	if(start==nullptr || end==nullptr)
		return -1;

	unordered_map<Vertex, bool> selected;
	unordered_map<Vertex, unsigned int> distances;

	list<Vertex*> Q;
	
	Vertex* node = vertices;
	//create queue and distances;
	while(node!=nullptr) {
		selected.insert({*node, false});
		if(contains_edge(start->name, node->name)) {
			distances.insert({*node, get_distance(start->name, node->name)});
			Q.push_back(node);
		}
		else {
			distances.insert({*node, numeric_limits<unsigned int>::max()});
		}
		node = node->next;
	}
	selected.at(*start)=true;

	while(not Q.empty()) {
		Vertex* u = Q.front();
		unsigned int min_dist = distances.at(*u);
		//select the vertex with min distance
		for(auto vert:Q) {
			if(distances.at(*vert)<min_dist) {
				min_dist = distances.at(*vert);
				u = vert;
			}
		}
		Q.remove(u);
		selected.at(*u)=true;
		Neighbour* v = u->neighbours;
		
		while(v!=nullptr) {
			//relax weights
			unsigned int new_dist = distances.at(*u) + get_distance(u->name, v->neighbour.name);
			if((not selected.at(v->neighbour)) and 
				distances.at(v->neighbour) > new_dist) {
				distances.at(v->neighbour) = new_dist;
				Q.push_back(getVertex(v->neighbour.name));
			}
			v=v->next;
		}
	}
	return distances.at(*end);
}
