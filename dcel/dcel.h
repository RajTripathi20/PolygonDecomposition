/*
Header File dcel.h
usage- #include "dcel.h"

Created a basic structure for dcel
Pending Tasks-
    1) Confirming the need for templates(coordinate data type unknown)
    2) More constructors for Edge class?
    3) Utility Functions in DCEL class based on problem requirement
    4) Confirming the usage for vectors for storing pointers in dcel(alternative- sets)
    5) Commenting for Doxygen
*/

/**

*@file dcel.h
*@brief Header file containing classes and functions for the Doubly-Connected Edge List (DCEL) data structure.

*/

template <class T> class Vertex;
template <class T> class Edge;
template <class T> class Face;

/**
 * @class Vertex
 * @brief Vertex Class for DCEL data structure
 */
template <class T>
class Vertex  {
private:
    string id;
    pair<T,T> coordinate;
    Edge<T>* incidentEdge;

public:
    //constructors

    /**
     * @brief Default constructor for Vertex
     */

    Vertex()  {

    }

    /**
     * @brief Constructs a new Vertex object with specified id and coordinate
     * @param id Unique identifier of the vertex
     * @param coordinate Coordinate of the vertex
     */

    Vertex(string id, pair<T,T> coordinate)  {
        this->id = id;
        this->coordinate = coordinate;
    }

    /**
     * @brief Vertex class constructor with id, coordinate, and incident edge.
     * @param id Unique identifier of the vertex
     * @param coordinate Coordinate of the vertex
     * @param incidentEdge Pointer to the incident edge of the vertex
     */

    Vertex(string id, pair<T,T> coordinate, Edge<T>* incidentEdge)  {
        this->id = id;
        this->coordinate = coordinate;
        this->incidentEdge = incidentEdge;
    }

    //getters

    /** 
     * @brief Returns unique ID associated with the vertex
     */
    string getId()  {
        return this->id;
    }

    /** 
     * @brief Returns coordinates of the vertex
     */

    pair<T,T> getCoordinate()  {
        return this->coordinate;
    }

    /** 
     * @brief Returns the edge incident on this vertex
     */

    Edge<T>* getIncidentEdge()  {
        return this->incidentEdge;
    }

    //setters

    /** 
     * @brief To update the ID
     */

    void setId(string id)  {
        this->id=id;
        return;
    }

    /** 
     * @brief To update the coordinates
     */

    void setCoordinate(pair<T,T> coordinate)  {
        this->coordinate=coordinate;
        return;
    }

    /** 
     * @brief To update the incident edge
     */
    
    void setIncidentEdge(Edge<T>* incidentEdge)  {
        this->incidentEdge=incidentEdge;
        return;
    }


};

/**
 * @brief Edge Class for DCEL data structure
 * A pointer to an origin Vertex of type Vertex<T>*.
 * A pointer to a left Face of type Face<T>*.
 * A pointer to a twin Edge of type Edge<T>*.
 * A pointer to the next Edge in the Face of type Edge<T>*.
 * A pointer to the previous Edge in the Face of type Edge<T>*.
 */

template <class T>
class Edge  {
private:
    Vertex<T>* originVertex;
    Face<T>* leftFace;
    Edge<T>* twinEdge;
    Edge<T>* nextEdge;
    Edge<T>* prevEdge;

public:
    //constructors
    /**
     * @brief Default Edge Constructor
     */
    Edge()  {

    }

    /**
     * @brief Constructs an Edge object starting from the given vertex
     * @param originVertex Origin Vertex
     */

    Edge(Vertex<T>* originVertex)  {
        this->originVertex=originVertex;
    }

    //getters

    /**
     * @brief Returns origin vertex
     */

    Vertex<T>* getOriginVertex()  {
        return this->originVertex;
    }

    /**
     * @brief Returns left face
     */
    Face<T>* getLeftFace()  {
        return this->leftFace;
    }

    /**
     * @brief Returns twin edge
     */

    Edge<T>* getTwinEdge()  {
        return this->twinEdge;
    }

    /**
     * @brief Returns next edge
     */

    Edge<T>* getNextEdge()  {
        return this->nextEdge;
    }

    /**
     * @brief Returns previous vertex
     */

    Edge<T>* getPrevEdge()  {
        return this->prevEdge;
    }

    //setters

    /**
     * @brief To update origin vertex
     */

    void setOriginVertex(Vertex<T>* originVertex)  {
        this->originVertex = originVertex;
        return;
    }

    /**
     * @brief To update leftface
     */

    void setLeftFace(Face<T>* leftFace)  {
        this->leftFace = leftFace;
        return;
    }

    /**
     * @brief To update twin edge
     */

    void setTwinEdge(Edge<T>* twinEdge)  {
        this->twinEdge = twinEdge;
        return;
    }

    /**
     * @brief To update next edge
     */

    void setNextEdge(Edge<T>* nextEdge)  {
        this->nextEdge = nextEdge;
        return;
    }

    /**
     * @brief To update previous edge
     */

    void setPrevEdge(Edge<T>* prevEdge)  {
        this->prevEdge = prevEdge;
        return;
    }

};

/**
 * @brief Face Class for DCEL data structure
 * A unique ID of type int.
 * A pointer to an incident Edge of type Edge<T>*.
 */

template <class T>
class Face  {
private:
    int id;
    Edge<T>* incidentEdge;

public:
    //constructors

    /**
     * @brief Default Face Constructor
     */

    Face()  {

    }

    /**
     * @brief Constructs Face Object with the list of edges given
     */

    Face(Edge<T>* incidentEdge)  {
        this->incidentEdge=incidentEdge;
    }

    /**
     * @brief Constructs Face Object with id and incident edge
     */

    Face(int id, Edge<T>* incidentEdge)  {
        this->id = id;
        this->incidentEdge=incidentEdge;
    }

    //getters
    
    /**
     * @brief Returns ID
     */

    int getId()  {
        return this->id;
    }

    /**
     * @brief Returns incident edge
     */

    Edge<T>* getIncidentEdge()  {
        return this->incidentEdge;
    }

    //setters
    /**
     * @brief Updates ID
     */

    void setId(int id)  {
        this->id=id;
        return;
    }

    /**
     * @brief Updates incident edge
     */

    void setIncidentEdge(Edge<T>* incidentEdge)  {
        this->incidentEdge=incidentEdge;
        return;
    }


};

/**
 * @brief Class template for a Doubly Connected Edge List (DCEL) data structure
 * The DCEL stores information about vertices, edges, and faces in a planar graph.
 * @tparam T 
 */

template <class T>
class DCEL  {
private:
vector<Vertex<T>*> vertices;
vector<Edge<T>*> edges;
vector<Face<T>*> faces;

public:
    //constructors

    /**
     * @brief Default Face Constructor
     */
    DCEL()  {

    }

    /**
     * @brief Constructs face with given vertices and edges
     */

    DCEL(vector<Vertex<T>*> vertices, vector<Edge<T>*> edges)  {
        this->vertices=vertices;
        this->edges=edges;
    }

    /**
     * @brief Constructs face with given vertices,edges,faces
     */

    DCEL(vector<Vertex<T>*> vertices, vector<Edge<T>*> edges, vector<Face<T>*> faces)  {
        this->vertices=vertices;
        this->edges=edges;
        this->faces = faces;
    }

    //getters

    /**
     * @brief Returns vertices
     */

    vector<Vertex<T>*> getVertices()  {
        return this->vertices;
    }

    /**
     * @brief Returns edges
     */
    vector<Edge<T>*> getEdges()  {
        return this->edges;
    }

    /**
     * @brief Returns faces
     */
    vector<Face<T>*> getFaces()  {
        return this->faces;
    }
    //setters

    //utility functions
    // void insertVertex(Vertex<T>* vert)  {
    //     this->vertices.insert(vert);
    // }
    // void removeVertex()  {
    //     this->vertices.insert(vert);
    // }

};