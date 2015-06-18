///--- hacked from OpenGP obj reader
#include <cstdio>
#include <Eigen/Dense>

template <class MatrixType>
bool read_obj(MatrixType& vertices, const std::string& filename) {
    char   s[200];
    float  x, y, z;
    
    // open file (in ASCII mode)
    FILE* in = fopen(filename.c_str(), "r");
    if (!in) return false;

    // clear line once
    memset(&s, 0, 200);
    
    //--- First pass, counts vertices
    int n_vertices = 0;
    while (in && !feof(in) && fgets(s, 200, in)) {
        // comment
        if (s[0] == '#' || isspace(s[0])) continue;   
        // vertex
        else if (strncmp(s, "v ", 2) == 0)
            n_vertices++;
    }
    fseek(in, 0, 0); ///< rewind
    // std::cout << "read #vertices: " << n_vertices << std::endl;
    vertices.resize(Eigen::NoChange,n_vertices);
    
    //--- Second pass, fills in
    int curr_vertex=0;
    while (in && !feof(in) && fgets(s, 200, in)) {
        // comment
        if (s[0] == '#' || isspace(s[0])) continue;

        // vertex
        else if (strncmp(s, "v ", 2) == 0) {
            if (sscanf(s, "v %f %f %f", &x, &y, &z)) {
                vertices(0,curr_vertex) = x;
                vertices(1,curr_vertex) = y;
                vertices(2,curr_vertex) = z;
                curr_vertex++;
            }
        }

        // normal
        else if (strncmp(s, "vn ", 3) == 0) {
            continue;
        }

        // face
        else if (strncmp(s, "f ", 2) == 0) {
            continue;
        }
        
        // clear line
        memset(&s, 0, 200);
    }

    fclose(in);
    return true;
}

//-----------------------------------------------------------------------------

///--- Replaces vertices in prev_filename with content of vertices, saves in filename
template <class MatrixType>
bool write_obj_replaceverts(const std::string& prev_filename, const MatrixType& vertices, const std::string& filename) {
    typedef Eigen::Vector3d Texture_coordinate;

    char   s[200];
    float  x, y, z;
    

    FILE* out = fopen(filename.c_str(), "w");
    FILE* in = fopen(prev_filename.c_str(), "r");
    if (!in || !out)
        return false;

    // clear line once
    memset(&s, 0, 200);
    
    //--- Second pass, fills in
    int curr_vertex=0;
    while (in && !feof(in) && fgets(s, 200, in)) {
        // vertex
        if (!isspace(s[0]) && strncmp(s, "v ", 2) == 0) {
            fprintf(out, "v %f %f %f\n", vertices(0,curr_vertex), vertices(1,curr_vertex), vertices(2,curr_vertex));
            curr_vertex++;
        } else {
            fprintf(out, "%s", s);            
        }
        
        // clear line
        memset(&s, 0, 200);
    }


    fclose(in);
    fclose(out);
    return true;
}
