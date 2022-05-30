#include "metis.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

typedef std::vector<idx_t> MetisIntList;
typedef std::vector<idx_t> vidx_t;

struct mgraph_t
{
	idx_t nvtxs, nedges;	/* The # of vertices and edges in the graph */
	idx_t ncon;		/* The # of constrains */
	idx_t* xadj;		/* Pointers to the locally stored vertices */
	idx_t* vwgt;		/* Vertex weights */
	idx_t* vsize;		/* Vertex sizes for min-volume formulation */
	idx_t* adjncy;        /* Array that stores the adjacency lists of nvtxs */
	idx_t* adjwgt;        /* Array that stores the weights of the adjacency lists */

	idx_t* tvwgt;         /* The sum of the vertex weights in the graph */
	real_t* invtvwgt;     /* The inverse of the sum of the vertex weights in the graph */
};

void ScalarPartitionByMetis( idx_t nCells, MetisIntList & xadj, MetisIntList & adjncy, int nPart, MetisIntList & cellzone )
{
	idx_t   ncon     = 1;
	idx_t   * vwgt   = 0;
	idx_t   * vsize  = 0;
	idx_t   * adjwgt = 0;
	float * tpwgts = 0;
	float * ubvec  = 0;
	idx_t options[ METIS_NOPTIONS ];
	idx_t wgtflag = 0;
	idx_t numflag = 0;
	idx_t objval;
	idx_t nZone = nPart;

	METIS_SetDefaultOptions( options );
	std::cout << "Now begining partition graph!\n";
	if ( nZone > 8 )
	{
		std::cout << "Using K-way Partitioning!\n";
		METIS_PartGraphKway( & nCells, & ncon, xadj.data(), adjncy.data(), vwgt, vsize, adjwgt,
			& nZone, tpwgts, ubvec, options, & objval, & cellzone[ 0 ] );
	}
	else
	{
		std::cout << "Using Recursive Partitioning!\n";
		METIS_PartGraphRecursive( & nCells, & ncon, xadj.data(), adjncy.data(), vwgt, vsize, adjwgt,
			& nZone, tpwgts, ubvec, options, & objval, & cellzone[ 0 ] );
	}
	std::cout << "The interface number: " << objval << std::endl; 
	std::cout << "Partition is finished!\n";
}

int main(int argc, char *argv[])
{
    std::cout << "metis test\n";
	namespace fs = std::filesystem;

	std::cout << "Current working directory: " << fs::current_path() << '\n';

	std::fstream file;
	file.open("../example_a.graph");
	std::string line;
	std::getline(file, line);
	int nvtxs = -1;
	int nedges = -1;
	int fmt = 0;
	//int fmt = 101;
	int ncon = -1;
	char fmtstr[256];

	int nfields = std::sscanf(line.c_str(), "%d %d %d %d", &nvtxs, &nedges, &fmt, &ncon);

	std::sprintf(fmtstr, "%03d", fmt % 1000);
	int readvs = (fmtstr[0] == '1');
	int readvw = (fmtstr[1] == '1');
	int readew = (fmtstr[2] == '1');

	int nedges2 = nedges * 2;

	vidx_t xadj, adjncy, vwgt, adjwgt, vsize;
	vidx_t cellzone;
	xadj.resize(nvtxs + 1);
	adjncy.resize(2 * nedges);
	adjwgt.resize(2 * nedges);

	xadj[0] = 0;
	int k = 0;

    for ( int i = 0; i < nvtxs; ++ i )
    {
        std::getline(file, line);

        // Read vertex sizes
        if ( readvs ) {
        }

        // Read vertex weights
        if ( readvw ) {
        }

		std::size_t pos{};

        while ( true ) {
			//std::stoi(const std::string & str, std::size_t * pos = nullptr, int base = 10);
			int edge = std::stoi(line, &pos);
		    line = line.substr(pos);
            if ( line == "" )
                break; /* End of line */
 
			if (edge < 1 || edge > nvtxs)
			{
				;
			}

            int ewgt = 1;
            if ( readew ) {
                ewgt = std::stoi(line, &pos);
            }

			if ( k == nedges2 )
			{
				exit(0);
			}

            adjncy[k] = edge - 1;
            adjwgt[k] = ewgt;
            k++;
        }
        xadj[i + 1] = k;
    }

	file.close();
	int nFaces = 1;
	int nCells = 1;
	int nBFaces =1;
	int nInnerFaces = nFaces - nBFaces;

	//vidx_t xadj, adjncy, vwgt, adjwgt, vsize;
	//vidx_t cellzone;
	//xadj.resize(nCells + 1);
	//adjncy.resize(2 * nInnerFaces);
	//cellzone.resize(nCells);

	int nPart = 1;

	ScalarPartitionByMetis(nCells, xadj, adjncy, nPart, cellzone);
    return 0;
}