/**
 * @file polygonDecomposition.cpp
 
 * @authors Dhruv Saxena    (f20191369@hyderabad.bits-pilani.ac.in)
            Pranjal Jasani  (f20190831@hyderabad.bits-pilani.ac.in)
            Raj Tripathi    (f20190869@hyderabad.bits-pilani.ac.in)
            Pranavi Gunda   (f20191068@hyderabad.bits-pilani.ac.in)     

 * @date 2023-03-07

 * @brief This code works on the problem of decomposing an arbitrary polygon into convex polygons,following a divide-and-conquer approach.
 * 
 * The code has been developed according to the algorithms mentioned in the following research paper:
 * 
 * Fernández, J., Cánovas, L., & Pelegrın, B. (2000). Algorithms for the decomposition of a polygon into 
 * convex polygons. European Journal of Operational Research, 121(2), 330-342.
 * 
 */ 

#include <bits/stdc++.h>
// #include <Python.h>
using namespace std;

#include "../dcel/dcel.h"
/*Notes
using a vector is problematic in returnPartition function if we do this clockwise
To do:
add the rectangle heuristic
merging two partitions into a polygon
rewrite the isInsidePolygon code

*/
// a vertex can have many edges incident on it, and it only carries info of 1
// but a edge originates from only 1 vertex. so this data is more reliable

/****************************************************/
ifstream fin;
ofstream fout;

/**
 * @brief A macro defining the data type used throughout the code as float. Can be replaced with other datatypes from here itself.
 */

#define T float

/**
 * @brief Alias for pair of T (i.e., pair<float,float>)
 */

#define Point pair<T,T>
// #define x first
// #define y second
#define inf 1000000

/**
 * @struct line
 * @brief Defined by the endpoints p1 and p2
 */
struct line {
    Point p1, p2;
};

/**
 * \brief Checks whether p is on l1 or not
 * \brief O(1) time and O(1) space
 * \param l1 line
 * \param p Point
 * \return True or False
 */

bool onLine(line l1, Point p)
{
    // Check whether p is on the line or not
    if (p.first <= max(l1.p1.first, l1.p2.first)
        && p.first >= min(l1.p1.first, l1.p2.first)
        && (p.second <= max(l1.p1.second, l1.p2.second)
            && p.second >= min(l1.p1.second, l1.p2.second)))
        return true;

    return false;
}

/**
 * \brief Determines orientation of points a,b,c wrt to each other
 * \brief O(1) time and O(1) space
 * \param a Point
 * \param b Point
 * \param c Point
 * \return An integer-0 for colinearity,1 for clockwise,2 for anticlockwise
 */

int direction(Point a, Point b, Point c)
{
    T val = (b.second - a.second) * (c.first - b.first)
              - (b.first - a.first) * (c.second - b.second);

    if (val == 0)

        // Colinear
        return 0;

    else if (val < 0)

        // Anti-clockwise direction
        return 2;

    // Clockwise direction
    return 1;
}

/**
 * \brief Determines point of intersection of lines l1 and l2
 * \brief O(1) time and O(1) space
 * \param l1 line
 * \param l2 line
 * \return Coordinates of the point
 */

pair<T,T> pointOfIntersection(line l1,line l2){
    double a1 = l1.p2.second - l1.p1.second;
    double b1 = l1.p1.first - l1.p2.first;
    double c1 = a1*(l1.p1.first) + b1*(l1.p1.second);

    double a2 = l2.p2.second - l2.p1.second;
    double b2 = l2.p1.first - l2.p2.first;
    double c2 = a2*(l2.p1.first)+ b2*(l2.p1.second);

    double determinant = a1*b2 - a2*b1;

    if (determinant == 0)
    {
        return make_pair(-inf,-inf);
    }
    else
    {
        double x = (b2*c1 - b1*c2)/determinant;
        double y = (a1*c2 - a2*c1)/determinant;
        return make_pair(x, y);
    }
}

// double distance(Point p1, Point p2){
//     return sqrt((p1.first-p2.first)*(p1.first-p2.first) + (p1.second-p2.second)*(p1.second-p2.second));
// }

// bool liesOnLine(Point p1, Point p2, Point p3){
//     if(distance(p1,p3)+distance(p3,p2)==distance(p1,p2)){
//         return true;
//     }
//     return false;
// }

/**
 * \brief Checks if two points p1 and p2 lie on the same or opposite sides of a line l3
 * \brief O(1) time and O(1) space
 * \param p1 Point
 * \param p2 Point
 * \param l3 line
 * \return True or False
 */

bool pointsAreOnSameSide(Point p1,Point p2,line l3){
    int dir1 = direction(l3.p1, l3.p2, p1);
    int dir2 = direction(l3.p1, l3.p2, p2);
    // cout<<"dir1: "<<dir1<<" dir2: "<<dir2<<" \n";
    if(dir1==dir2){
        return true;
    }
    return false;
}

/**
 * \brief Checks if the three lines l1,l2,l3 intersect
 * \brief O(1) time,O(1) space
 * \param l1 line
 * \param l2 line
 * \param l3 line
 * @return 0 if the three line segments do not intersect
 *         1 if the three line segments intersect at three different points
 *         2 if the two lines which form the edge of the polygon, intersect the line from a point in the polygon to infinity at a vertex of the polygon provided the edges lie on different sides of the line
 *         3 if the two lines which form the edge of the polygon, intersect the line from a point in the polygon to infinity at a vertex of the polygon provided the edges lie on the same side of the line
 */

int isIntersect(line l1, line l2,line l3)
{
    // Four direction for two lines and points of other line
    int dir1 = direction(l1.p1, l1.p2, l2.p1);
    int dir2 = direction(l1.p1, l1.p2, l2.p2);
    int dir3 = direction(l2.p1, l2.p2, l1.p1);
    int dir4 = direction(l2.p1, l2.p2, l1.p2);

    // When intersecting
    Point intersectionPoint1=pointOfIntersection(l1,l2);
    Point intersectionPoint2=pointOfIntersection(l2,l3);
    if(intersectionPoint1.first==intersectionPoint2.first && intersectionPoint1.second==intersectionPoint2.second){
        if (dir1 != dir2 && dir3 != dir4){
            // cout<<"Points are "<<l1.p1.first<<" "<<l1.p1.second<<" "<<l2.p1.first<<" "<<l2.p1.second<<" "<<l2.p2.first<<" "<<l2.p2.second<<" "<<l3.p2.first<<" "<<l3.p2.second<<" \n";
            if(pointsAreOnSameSide(l1.p1,l3.p2,l2)){
                return 3;
            }
            return 2;
        }

        // When p2 of line2 are on the line1
        if (dir1 == 0 && onLine(l1, l2.p1)){
            if(pointsAreOnSameSide(l1.p1,l3.p2,l2)){
                return 3;
            }
            return 2;
        }

        // When p1 of line2 are on the line1
        if (dir2 == 0 && onLine(l1, l2.p2)){
            if(pointsAreOnSameSide(l1.p1,l3.p2,l2)){
                return 3;
            }
            return 2;
        }

        // When p2 of line1 are on the line2
        if (dir3 == 0 && onLine(l2, l1.p1)){
            if(pointsAreOnSameSide(l1.p1,l3.p2,l2)){
                return 3;
            }
            return 2;
        }

        // When p1 of line1 are on the line2
        if (dir4 == 0 && onLine(l2, l1.p2)){
            if(pointsAreOnSameSide(l1.p1,l3.p2,l2)){
                return 3;
            }
            return 2;
        }

        return 0;
    }
    else{
        if (dir1 != dir2 && dir3 != dir4){
            return 1;
        }

        // When p2 of line2 are on the line1
        if (dir1 == 0 && onLine(l1, l2.p1)){
            return 1;
        }

        // When p1 of line2 are on the line1
        if (dir2 == 0 && onLine(l1, l2.p2)){
            return 1;
        }

        // When p2 of line1 are on the line2
        if (dir3 == 0 && onLine(l2, l1.p1)){
            return 1;
        }

        // When p1 of line1 are on the line2
        if (dir4 == 0 && onLine(l2, l1.p2)){
            return 1;
        }

        return 0;
    }

}

/**
 * \brief Constructs a rectangle around the polygon to help verify isInsidePolygon if other points lie inside the partition created
 * \brief O(n) where n is the number of vertices in the polygon, O(1) space
 * \param &partitionVertices Pointer to the vertices of the polygon
 * \return Co-ordinates of the lower left vertex and upper right vertex
 */

pair<Point,Point> getRectangleAroundPolygon(vector<Vertex<T>*> &partitionVertices){
    double minX=inf;
    double minY=inf;
    double maxX=-inf;
    double maxY=-inf;
    for(int i=0;i<partitionVertices.size();i++){
        if(partitionVertices[i]->getCoordinate().first<minX){
            minX=partitionVertices[i]->getCoordinate().first;
        }
        if(partitionVertices[i]->getCoordinate().first>maxX){
            maxX=partitionVertices[i]->getCoordinate().first;
        }
        if(partitionVertices[i]->getCoordinate().second<minY){
            minY=partitionVertices[i]->getCoordinate().second;
        }
        if(partitionVertices[i]->getCoordinate().second>maxY){
            maxY=partitionVertices[i]->getCoordinate().second;
        }
    }
    return make_pair(make_pair(minX,minY),make_pair(maxX,maxY));
}

/**
 * \brief Tells whether other points(not part of the partition) are inside the partition polygon
 * \brief O(n) time where n is the number of vertices in the partition polygon, O(1) spacee
 * \param &partitionVertices Partition Vertices
 * \param &partitionEdges Partition Edges
 * \param vertex Vertex
 * \return True or False
 */

bool isInsidePolygon(vector<Vertex<T>*> &partitionVertices, vector<Edge<T>*> &partitionEdges,Vertex<T>* vertex)  {

// bool checkInside(Point poly[], int n, Point p)
    int n = partitionVertices.size();
    // When polygon has less than 3 edge, it is not polygon
    if (n < 3)
        return false;
    // Create a point at infinity, y is same as point p
    line exline = { vertex->getCoordinate(), { inf, vertex->getCoordinate().second } };
    int count = 0;
    int i = 0;
    do {

        // Forming a line from two consecutive points of
        // poly
        // line side = { poly[i], poly[(i + 1) % n] };
        line side = {partitionVertices[i]->getCoordinate(), partitionVertices[(i+1)%n]->getCoordinate()};
        line nextSide = {partitionVertices[(i+1)%n]->getCoordinate(), partitionVertices[(i+2)%n]->getCoordinate()};
        // cout<<"Side is "<<partitionVertices[i]->getId()<<" "<<partitionVertices[(i+1)%n]->getId()<<endl;
        // cout<<"Next Side is "<<partitionVertices[(i+1)%n]->getId()<<" "<<partitionVertices[(i+2)%n]->getId()<<endl;
        if (isIntersect(side, exline,nextSide)==1) {
            // cout<<"Intersected Line "<<partitionVertices[i]->getId()<<" "<<partitionVertices[(i+1)%n]->getId()<<endl;
            // If side is intersects exline
            if (direction(side.p1, vertex->getCoordinate(), side.p2) == 0)
                return onLine(side, vertex->getCoordinate());
            count++;
        }
        else if (isIntersect(side, exline,nextSide)==2) {
            // cout<<"Intersected Line "<<partitionVertices[i]->getId()<<" "<<partitionVertices[(i+1)%n]->getId()<<endl;
            // If side is intersects exline
            if (direction(side.p1, vertex->getCoordinate(), side.p2) == 0)
                return onLine(side, vertex->getCoordinate());
        }
        else if (isIntersect(side, exline,nextSide)==3) {
            // cout<<"Intersected Line "<<partitionVertices[i]->getId()<<" "<<partitionVertices[(i+1)%n]->getId()<<endl;
            // If side is intersects exline
            if (direction(side.p1, vertex->getCoordinate(), side.p2) == 0)
                return onLine(side, vertex->getCoordinate());
            count--;
        }
        // cout<<"COUNT "<<count<<endl;
        i = (i + 1) % n;
    } while (i != 0);

    // When count is odd
    return count & 1;
}
/****************************************************************/

/**
 * \brief Tells you whether the angle subtended v1v2v3 in clockwise order is reflex(ie >180)
 * \brief O(1) time and O(1) space
 * \param vertex1 vertex1
 * \param vertex2 vertex2
 * \param vertex3 vertex3
 * \return true or false
 */

bool isReflex(Vertex<T>* vertex1, Vertex<T>* vertex2, Vertex<T>* vertex3)  {
    pair<T,T> c0 = vertex1->getCoordinate();
    pair<T,T> c1 = vertex2->getCoordinate();
    pair<T,T> c2 = vertex3->getCoordinate();

    T x1 = c0.first-c1.first;
    T y1 = c0.second-c1.second;

    T x2 = c2.first-c1.first;
    T y2 = c2.second-c1.second;

    return (x1*y2-x2*y1)<0;
}

/**
 * \brief Prints the polygon
 * \brief O(n) time where n is the number of edges in the polygon and O(1) space
 * \param edge edge
 * \return none
 */

void printPolygon(Edge<T>* edge)  {
    fout<<"Printing vertices in clockwise order\n";
    Edge<T>* fixEdge=edge;
    do{
        fout<<edge->getOriginVertex()->getId()<<" "<<edge->getOriginVertex()->getCoordinate().first<<" "<<edge->getOriginVertex()->getCoordinate().second<<"\n";
        edge=edge->getNextEdge();
    }  while(edge!=fixEdge);
    // for(int i = 0; i<vertices.size(); i++)  {
    //     fout<<vertices[i]->getId()<<" "<<vertices[i]->getCoordinate().first<<" "<<vertices[i]->getCoordinate().second<<"\n";
    // }
    fout<<"\n\n\n";
}

/**
 * \brief Prints the final decomposition with the added edges in the original polygon
 * \brief O(n) where n is the number of vertices, O(1) space complexity
 * \param decomposition decompositon
 * \return none
 */

void printDecompositon(DCEL<T>* decompositon)  {
    fout<<"Printing faces in clockwise order\n";
    for(auto face:decompositon->getFaces())  {
        Edge<T>* edge=face->getIncidentEdge(), *fixEdge = face->getIncidentEdge();
        fout<<"Face "<<face->getId()<<"\n";
        do  {
            fout<<edge->getOriginVertex()->getId()<<" "<<edge->getOriginVertex()->getCoordinate().first<<" "<<edge->getOriginVertex()->getCoordinate().second<<"\n";
            edge=edge->getNextEdge();
        }while(edge!=fixEdge);
    fout<<"\n\n";
    }
    fout<<"\n";
}

/**
 * \brief Constructs the rectangle given the coordinates of opposite corner points p1 and p2
 * \brief O(1) time and O(1) space
 * \param p1 Point
 * \param p2 Point
 * \return Pointer to the DCEL object representing the rectangle
 */

DCEL<T>* makeRectangle(Point p1,Point p2){
    vector<Vertex<T>*> vertices;
    vector<Edge<T>*> edges;
    string id;
    pair<T,T> coordinate;
    for(int i=0;i<4;i++){
        id = to_string(i);
        if(i==0){
            coordinate = make_pair(p1.first,p1.second);
        }
        else if(i==1){
            coordinate = make_pair(p2.first,p1.second);
        }
        else if(i==2){
            coordinate = make_pair(p2.first,p2.second);
        }
        else if(i==3){
            coordinate = make_pair(p1.first,p2.second);
        }

        Vertex<T>* vertex = new Vertex<T>(id, coordinate,new Edge<T>(vertex));
        vertices.push_back(vertex);
        edges.push_back(vertex->getIncidentEdge());

        if(i>=1)  {
            edges[i-1]->setNextEdge(edges[i]);
            edges[i]->setPrevEdge(edges[i-1]);
        }
        if(i==3)  {
            edges[i]->setNextEdge(edges[0]);
            edges[0]->setPrevEdge(edges[i]);
        }
    }
    DCEL<T>* rectangle = new DCEL<T>(vertices,edges);
    return rectangle;
}

// int findGlobalIndex(Vertex<T>* vertex,vector<Vertex<T>*> originalPolygonVertices){
//     for(int i=0;i<originalPolygonVertices.size();i++){
//         if(originalPolygonVertices[i]->getId()==vertex->getId()){
//             return i;
//         }
//     }
//     return -1;
// }

// bool isNotch(const vector<Vertex<T>*> &originalPolygonVertices,int i){
//     int n=originalPolygonVertices.size();
//     // cout<<"Checking for notch at "<<i<<"th vertex\n"<<(n+i-1)%n<<" "<<i<<" "<<(i+1)%n<<"\n";

//     return isReflex(originalPolygonVertices,(n+i-1)%n,i,(i+1)%n);
// }

//add the condition to try again with different vertex if no polygon found

/**
 * \brief Returns a convex partition and updates the original polygon to the relative complement of this partition
 * \brief Worst Case Time Complexity- O(n^2) where n is the number of vertices in the original polygon
 * \brief Worst Case Space Complexity- O(n) where n is the number of vertices in the original polygon
 * \param  &vertices A vector of vertices holding the input polygon
 * \param &edges A vector of edges holding the connections between the vertices
 * \param start index of the starting vertex in the input polygon
 * \param countPartition # of the current Partition
 * \return Pointer to the DCEL object of the partition
 */

DCEL<T>* returnPartitionAndUpdatePolygon(vector<Vertex<T>*> &vertices, vector<Edge<T>*> &edges, int start, int countPartition)  {
    if(vertices.size()<=2)
        return NULL;
    DCEL<T>* partitionPolygon; //ans

    int n = vertices.size();
    vector<Vertex<T>*> partitionVertices;
    vector<Edge<T>*> partitionEdges;

    set<string> isRemovedFromMainPolygon;

    partitionVertices.push_back(vertices[start%n]);
    isRemovedFromMainPolygon.insert(vertices[start%n]->getId());

    partitionVertices.push_back(vertices[(start+1)%n]);
    isRemovedFromMainPolygon.insert(vertices[(start+1)%n]->getId());

    partitionEdges.push_back(edges[start%n]);
    //


    // fout<<"here 151\n";
    int i = (start+1)%n;
    //add provisional vertices
    while(!isReflex(vertices[((i-1)%n+n)%n], vertices[(i)%n], vertices[(i+1)%n]) && !isReflex(vertices[(i)%n], vertices[(i+1)%n], vertices[(start)%n]) && !isReflex(vertices[(i+1)%n], vertices[start%n], vertices[(start+1)%n]))  {
        if((i+1)%n==start%n)  {
            partitionEdges.push_back(edges[(i)%n]);
            // partitionEdges[0]->setPrevEdge(partitionEdges.back());
            // partitionEdges.back()->setNextEdge(partitionEdges[0]);
            break;
        }
        partitionVertices.push_back(vertices[(i+1)%n]);
        isRemovedFromMainPolygon.insert(vertices[(i+1)%n]->getId());
        partitionEdges.push_back(edges[(i)%n]);
        i=i%n;
        i++;
    }
    // fout<<"here 165\n";
    // printPolygon(partitionEdges[0]);
    // if(partitionVertices.size())
    //index of the last provisional edge in original polygon
    // int countForConnections=(i)%n;
    Edge<T>* nextEdge = partitionEdges.back()->getNextEdge();
    Edge<T>* prevEdge = partitionEdges[0]->getPrevEdge();
    if((i+1)%n!=start%n&&partitionVertices.size()>2)  {
        Edge<T>* edge = new Edge<T>(partitionVertices.back());
        // partitionVertices.back()->setIncidentEdge(edge);
        edge->setNextEdge(partitionEdges[0]);
        edge->setPrevEdge(partitionEdges.back());
        partitionEdges[0]->setPrevEdge(edge);
        partitionEdges.back()->setNextEdge(edge);
        partitionEdges.push_back(edge);
    }
    // fout<<"here 174\n";
    // printPolygon(partitionEdges[0]);
    //check if the provisional polygon is valid
    // vector<Vertex<T>*> originalPolygonVertices = originalPolygon->getVertices();

    pair<Point,Point> rectangleCoordinates=getRectangleAroundPolygon(partitionVertices);
    DCEL<T>* rectangle = makeRectangle(rectangleCoordinates.first,rectangleCoordinates.second);

    vector<Vertex<T>*> rectangleVertices = rectangle->getVertices();
    vector<Edge<T>*> rectangleEdges = rectangle->getEdges();

    bool isFirst = true;
    for(int i = 0; i<n&&partitionVertices.size()>2; i++)  {
        //point in P-L and L has more than 2 points
        if(isRemovedFromMainPolygon.find(vertices[i]->getId())==isRemovedFromMainPolygon.end())  { //not removed
            //if some point of P-L is which is a notch is inside the rectangle of the polygon
            //and is inside then polygon, then
            if(isReflex(vertices[((i-1)%n+n)%n], vertices[(i)%n], vertices[(i+1)%n]) && isInsidePolygon(rectangleVertices,rectangleEdges,vertices[i]) && isInsidePolygon(partitionVertices,partitionEdges,vertices[i]))  {
                // fout<<"removing "<<partitionVertices.back()->getId()<<" because of "<<vertices[i]->getId()<<"\n";
                //put the vertex back in the main polygon
                isRemovedFromMainPolygon.erase(partitionVertices.back()->getId());
                //remove the vertex from the provisional polygon
                partitionVertices.pop_back();

                if(partitionVertices.size()==2)  {
                    partitionEdges.pop_back();
                    partitionEdges[0]->setPrevEdge(prevEdge);
                    partitionEdges.back()->setNextEdge(nextEdge);
                    break;  
                }
                // if(isFirst)  {
                //     delete edge;
                //     isFirst = false;  
                // }
                // else  {
                //     edge->setNextEdge(nextEdge);  
                    
                //     nextEdge = edge;
                // }
                // partitionEdges.back()->setNextEdge(partitionEdges[0]);
                // partitionEdges[0]->setPrevEdge(partitionEdges.back());

                //diag edge prev becomes its prev->prev
                partitionEdges.back()->setPrevEdge(partitionEdges[partitionEdges.size()-3]);
                //diag edge origin vertex is the new last vertex
                partitionEdges.back()->setOriginVertex(partitionVertices.back());
                //remove the second last edge
                swap(partitionEdges[partitionEdges.size()-2],partitionEdges.back());
// /**************/
                partitionEdges.back()->setNextEdge(nextEdge);
                nextEdge->setPrevEdge(partitionEdges.back());
                nextEdge = partitionEdges.back();
// /**************/

                partitionEdges.pop_back();
                partitionEdges[partitionEdges.size()-2]->setNextEdge(partitionEdges.back());


                // redundant
                // partitionEdges.back()->setNextEdge(partitionEdges[0]);
                // partitionEdges[0]->setPrevEdge(partitionEdges.back());

                rectangleCoordinates=getRectangleAroundPolygon(partitionVertices);
                rectangle = makeRectangle(rectangleCoordinates.first,rectangleCoordinates.second);

                rectangleVertices = rectangle->getVertices();
                rectangleEdges = rectangle->getEdges();

                i=-1;
            }
        }
    }
    
    // fout<<"here 196\n";
    // printPolygon(partitionEdges[0]);
    //convert the polygon into dcel
    if(partitionVertices.size()<=2)
        return NULL;
    Face<T>* face = new Face<T>(countPartition, partitionEdges[0]);
    vector<Face<T>*> faces;
    faces.push_back(face);
    partitionPolygon = new DCEL<T>(partitionVertices, partitionEdges, faces);


    // create new vertex list and edge list and update original polygon
    if(partitionVertices.size()==n)  {
        vertices.clear();
        edges.clear();
    }
    else  {
        vector<Vertex<T>*> verticesNew;
        vector<Edge<T>*> edgesNew;
        //start from the last vertex of decomposition to the first vertex of decomposition
        i = start+partitionVertices.size()-1;
        while(i%n!=start%n)  {
            verticesNew.push_back(vertices[i%n]);
            edgesNew.push_back(edges[i%n]);
            i=i%n;
            i++;
        }
        verticesNew.push_back(vertices[start%n]); // first vertex of decomposition
        Edge<T>* edge = new Edge<T>(verticesNew.back());
        // partitionVertices.back()->setIncidentEdge(edge);
        edge->setPrevEdge(edgesNew.back());
        edge->setNextEdge(edgesNew[0]);
        edgesNew[0]->setPrevEdge(edge);
        edgesNew.back()->setNextEdge(edge);
        edgesNew.push_back(edge);

        vertices = verticesNew;
        edges = edgesNew;
    }
    // fout<<"here 230\n";
    //return dcel
    return partitionPolygon;
}  

/**
 * \brief Merges the partition polygon with the polygon decomposition and checks if removing any common edges leads to a convex polygon. If yes,it removes that edge and returns the polygon decomposition.
 * \brief O(n) time (where n is the number of vertices), O(n) space(where n is the number of vertices) 
 * \param partitionPolygon Partition of polygon
 * \param polygonDecomposition Decomposition of a polygon
 * \return Pointer to the resulting merged DCEL object
 */

DCEL<T>* mergePartitionWithDecomposition(DCEL<T>* partitionPolygon, DCEL<T>* polygonDecomposition, int &countPartition)  {
    //the first time the function is called
    if(!polygonDecomposition)
        return partitionPolygon;

    //the last time the function is called
    if(!partitionPolygon)
        return polygonDecomposition;

    //initialisations
    vector<Vertex<T>*> partitionVertices=partitionPolygon->getVertices();
    vector<Vertex<T>*> decompositonVertices=polygonDecomposition->getVertices();

    vector<Edge<T>*> partitionEdges=partitionPolygon->getEdges();
    vector<Edge<T>*> decompositionEdges=polygonDecomposition->getEdges();

    vector<Face<T>*> partitionFaces=partitionPolygon->getFaces();
    vector<Face<T>*> decompositionFaces=polygonDecomposition->getFaces();

    //to find common pair of vertices, and access the edge between
    map<pair<Vertex<T>*,Vertex<T>*>, Edge<T>*> vertexPairs;

    //for faster lookup on vertices
    set<Vertex<T>*> decompositonVerticesSet;
    //for marking non essential diagonal edges
    set<Edge<T>*> isEdgeMarkedForRemoval;
    //if a new face is to be added
    bool addNewFace=true;

    //get all the pair of vertices that have an edge between them in the partition
    for(auto edge: partitionEdges)  {
        vertexPairs[{edge->getOriginVertex(), edge->getNextEdge()->getOriginVertex()}]=edge;
    }

    //to have faster look up time
    for(auto vertex: decompositonVertices)  {
        decompositonVerticesSet.insert(vertex);
    }

    //each face is a cycle. so every face works as a distinct polygon
    for(auto face: decompositionFaces)  {
            Edge<T>* edge = face->getIncidentEdge(), *fixEdge = face->getIncidentEdge();
            //for iterating the face clockwise
            vector<Edge<T>*> edgeIterator;
            //doing this first because we'll be updating the connections
            do{
                edgeIterator.push_back(edge);
                edge=edge->getNextEdge();
            } while(edge!=fixEdge);

            for(auto edge:edgeIterator)  {
            //found a common pair of vertices, i.e a common diagonal
            if(vertexPairs.find({edge->getNextEdge()->getOriginVertex(), edge->getOriginVertex()})!=vertexPairs.end()) {
               
               Edge<T>* partitionEdge=vertexPairs[{edge->getNextEdge()->getOriginVertex(), edge->getOriginVertex()}];
               //if found a common diagonal check for reflex angles on the vertices
                if(!isReflex(partitionEdge->getPrevEdge()->getOriginVertex(), partitionEdge->getOriginVertex(), edge->getNextEdge()->getNextEdge()->getOriginVertex())  && !isReflex(edge->getPrevEdge()->getOriginVertex(), edge->getOriginVertex(), partitionEdge->getNextEdge()->getNextEdge()->getOriginVertex()))  {

                    //change the marker for face if necessary
                    if(face->getIncidentEdge()==edge || face->getIncidentEdge()==partitionEdge)
                        face->setIncidentEdge(edge->getNextEdge());

                    //remove the diagonal, connect the new cycle
                    (partitionEdge->getPrevEdge())->setNextEdge(edge->getNextEdge());
                    (edge->getPrevEdge())->setNextEdge(partitionEdge->getNextEdge());

                    (partitionEdge->getNextEdge())->setPrevEdge(edge->getPrevEdge());
                    (edge->getNextEdge())->setPrevEdge(partitionEdge->getPrevEdge());
                    //no additional face is added(at most one new face is to be added)
                    addNewFace=false;
                    //mark the pair of edges to not be added
                    isEdgeMarkedForRemoval.insert(edge);
                    isEdgeMarkedForRemoval.insert(partitionEdge);

                    //if their vertices had these incident, change that
                    if(edge->getOriginVertex()->getIncidentEdge()==edge)  {
                        edge->getOriginVertex()->setIncidentEdge(partitionEdge->getNextEdge());
                    }
                    if(partitionEdge->getOriginVertex()->getIncidentEdge()==partitionEdge)  {
                        partitionEdge->getOriginVertex()->setIncidentEdge(edge->getNextEdge());
                    }
                }
                //the diagonal is essential
                else  {
                    //make the edges a twin of each other, done.
                    edge->setTwinEdge(partitionEdge);
                    partitionEdge->setTwinEdge(edge);
                }
            }
            //no common diagonal found
            else  {
                continue;
            }
        }
    }
    vector<Vertex<T>*> finalDecompositionVertices;
    vector<Edge<T>*> finalDecompositionEdges;
    vector<Face<T>*> finalDecompositionFaces;

    //add all edges not marked
    for(auto edge: decompositionEdges)  {
        //not marked for removal
        if(isEdgeMarkedForRemoval.find(edge)==isEdgeMarkedForRemoval.end())
            finalDecompositionEdges.push_back(edge);
    }
    for(auto edge: partitionEdges)  {
        //not marked for removal
        if(isEdgeMarkedForRemoval.find(edge)==isEdgeMarkedForRemoval.end())
            finalDecompositionEdges.push_back(edge);
    }

    //add face is new face is to be added
    finalDecompositionFaces=decompositionFaces;
    //there is only one face in the partition
    if(addNewFace)
        finalDecompositionFaces.push_back(partitionFaces[0]);

    //union of vertices
    finalDecompositionVertices=decompositonVertices;
    for(auto vertex: partitionVertices)  {
        //not found in the set
        if(decompositonVerticesSet.find(vertex)==decompositonVerticesSet.end())
            finalDecompositionVertices.push_back(vertex);
    }

    DCEL<T>* finalDecomposition = new DCEL<T>(finalDecompositionVertices, finalDecompositionEdges, finalDecompositionFaces);
    if(!addNewFace)
        countPartition--;
    return finalDecomposition;
}


/**
 * \brief Takes a DCEL representation of a polygon as input and converts it into an adjacency matrix and a list of vertices.
 * \param O(n^2) time where n is the number of vertices, O(n^2) space where n is the number of vertices 
 * \param polygonDCEL DCEL object denoting polygon
 * \param &adjacencyMatrix 2D vector that will be used to store the adjacency matrix of the polygon as an undirected graph
 * \param &vertexList a reference to a vector of Points that will be used to store the coordinates of the vertices of the polygon in the same order as they appear in the adjacencyMatrix.
 * \return 
 */

void convertDCELtoGraph(DCEL<T>* polygonDCEL, vector<vector<int>> &adjacencyMatrix, vector<Point> &vertexList)  {
    //load vertices
    vector<Vertex<T>*> vertices=polygonDCEL->getVertices();
    //initialisation
    vector<int> rowAdjacency(vertices.size(), 0);
    for(int i = 0; i<vertices.size(); i++)  {
        adjacencyMatrix.push_back(rowAdjacency);
    }

    //for fast look up and easy filling
    map<Point,int> coordinateIndex;
    for(int i = 0; i<vertices.size(); i++)  {
        vertexList.push_back(vertices[i]->getCoordinate());
        coordinateIndex[vertices[i]->getCoordinate()]=i;
    }

    for(auto face:polygonDCEL->getFaces())  {
        Edge<T>* edge=face->getIncidentEdge(), *fixEdge = face->getIncidentEdge();
        do  {
            adjacencyMatrix[coordinateIndex[edge->getOriginVertex()->getCoordinate()]][coordinateIndex[edge->getNextEdge()->getOriginVertex()->getCoordinate()]]=1;
            adjacencyMatrix[coordinateIndex[edge->getNextEdge()->getOriginVertex()->getCoordinate()]][coordinateIndex[edge->getOriginVertex()->getCoordinate()]]=1;
            edge=edge->getNextEdge();
        }while(edge!=fixEdge);
    }
    return;
}

/**
 * \brief Prints graph:prints the graph represented by the given list of vertices and adjacency matrix
 * \brief O(n^2) time complexity where n is the number of vertices in the vertex list, O(1) space complexity
 * \param &vertexList A reference to a list carrying coordinates of the vertices
 * \param &adjacencyMatrix Pointer to Adjacency Matrix
 * \return None
 */

void printGraph(vector<Point> &vertexList, vector<vector<int>> &adjacencyMatrix)  {
    fout<<"Coordinates are:\n";
    for(auto point: vertexList)  {
        fout<<"("<<point.first<<","<<point.second<<")\n";
    }
    fout<<"Draw edges between the following pairs:\n";
    for(int i = 0; i<vertexList.size(); i++)  {
        for(int j = 0; j<i; j++)  {
            if(adjacencyMatrix[i][j]==1)  {
            fout<<"("<<vertexList[i].first<<","<<vertexList[i].second<<")---";
            fout<<"("<<vertexList[j].first<<","<<vertexList[j].second<<")\n";
            }
        }
    }
    fout<<"\n\n\n";
    return;
}

// Declare this function outside
// the main function

/**
 * \brief Utility function to handle input/output files
 */

void local(int i)
{
    string s;
    if(i>9)
        s.push_back('0'+i/10);
    s.push_back('0'+i%10);
  // In case of online judges (like
  // codechef, codeforces etc) these
  // lines will be skipped. In other
  // words these lines would be executed
  // in Sublime Text only
  // #ifndef ONLINE_JUDGE

  fin.open("../Inputs/inputDemo"+s+".txt");
  fout.open("../Outputs/outputDemo"+s+".txt");

  // ONLINE_JUDGE
  // #endif
}
/**
 * @brief Creates an output file which acts as an input for visualise.py which visualises the output
 */

void localOut(int i)
{
    string s;
    if(i>9)
        s.push_back('0'+i/10);
    s.push_back('0'+i%10);
  // In case of online judges (like
  // codechef, codeforces etc) these
  // lines will be skipped. In other
  // words these lines would be executed
  // in Sublime Text only
  // #ifndef ONLINE_JUDGE
  fout.open("../Visualisations/Inputs/visualisationDemo"+s+"_ans.txt");

  // ONLINE_JUDGE
  // #endif
}

/**
 * @brief To dump down the inputs for the visualiser in a file
 * @brief O(n^2) time complexity where n is the number of vertices, O(1) space complexity
 * @param vertices List of Vertices 
 * @param adjacencyMatrix Adjacency Matrix
 * @param i Index to create a unique file name in each call
 * \return None
 */

void visualise(vector<Vertex<T>*> vertices, vector<vector<int>> &adjacencyMatrix, int i)  {

    localOut(i);

    fout<<vertices.size()<<"\n";
    for(auto vertex: vertices)  {
        fout<<vertex->getId()<<" "<<vertex->getCoordinate().first<<" "<<vertex->getCoordinate().second<<"\n";
    }
    for(int i = 0; i<vertices.size(); i++)  {
        for(int j = 0; j<vertices.size(); j++)  {
            fout<<adjacencyMatrix[i][j]<<" ";
        }
        fout<<"\n";
    }
    fout.close();
    return;
}

// void runPythonScript(int i)  {
//     string s;
//     if(i>9)
//         s.push_back('0'+i/10);
//     s.push_back('0'+i%10);

//     FILE* file;
//     int argc;
//     char * argv[2];

//     argc = 2;
//     argv[0] = "visualise.py";
//     argv[1] = s;
//     Py_SetProgramName(argv[0]);
//     Py_Initialize();
//     PySys_SetArgv(argc, argv);
//     file = fopen("visualise.py","r");
//     PyRun_SimpleFile(file, "visualise.py");
//     Py_Finalize();
//     fclose(file);
// }

/**
 * @brief Creates a deep copy of the vertices and edges, and store them in the non-constant vectors passed as arguments.
 * @brief O(n) time where n is the number of vertices, O(n) space
 * @param vertices a reference to a vector of pointers to Vertex objects
 * @param edges a reference to a vector of pointers to Edge objects
 * @param verticesConst new vector of pointers to Vertex objects
 * @param edgesConst new vector of pointers to Edge objects
 */
void deepCopy(vector<Vertex<T>*> &vertices, vector<Edge<T>*> &edges,  vector<Vertex<T>*> verticesConst, vector<Edge<T>*> edgesConst)  {
    vertices.clear();// = deepCopy<Vertex<T>*>(verticesConst);
    edges.clear();// = deepCopy<Edge<T>*>(edgesConst);
    for(int i = 0; i<verticesConst.size(); i++)  {
        string id=verticesConst[i]->getId();
        pair<T,T> coordinate=verticesConst[i]->getCoordinate();
        Edge<T>* edge = new Edge<T>();
        Vertex<T>* vertex = new Vertex<T>(id, coordinate, edge);
        edge->setOriginVertex(vertex);
        vertices.push_back(vertex);
        edges.push_back(edge);

        //at least 2 vertices are there, so we can start making edge connections
        // not making the graph "directed" by splitting edges, since not required
        if(i>=1)  {
            edges[i-1]->setNextEdge(edges[i]);
            edges[i]->setPrevEdge(edges[i-1]);
        }
        if(i==verticesConst.size()-1)  {
            edges[i]->setNextEdge(edges[0]);
            edges[0]->setPrevEdge(edges[i]);
        }
    }
}

/**
 * \brief To check if the two polygons p1 and p2 are the same
 * \brief O(n) time where n=number of vertices in polygon1=number of vertices in polygon 2,O(1) auxiliary space
 */

bool polygonCompare(DCEL<T>* polygon1, DCEL<T>* polygon2)  {
    auto vertices1 = polygon1->getVertices()[0];
    auto vertices2 = polygon2->getVertices()[0];
    auto size1 = polygon2->getVertices().size();
    auto size2 = polygon2->getVertices().size();

    if(size1!=size2)
        return false;

    int i = 0;
    while(i<size2 && vertices1->getId()!=vertices2->getId())  {
        vertices2=vertices2->getIncidentEdge()->getNextEdge()->getOriginVertex();
        i++;
    }
    if(i==size2)
        return false;
    i=0;
    while(i<size2 && vertices1->getId()==vertices2->getId())  {
        vertices1=vertices1->getIncidentEdge()->getNextEdge()->getOriginVertex();
        vertices2=vertices2->getIncidentEdge()->getNextEdge()->getOriginVertex();
        i++;
    }
    if(i==size2)
        return true;
    else
        return false;
}

/**
 * @brief program to convert a given "polygon" into a set of convex polygons without introducing additional vertices
 * @brief Worst Case Time Complexity- O(n^4) where n is the number of vertices in the original polygon
 * @brief Worst Case Space Complexity- O(n^2) where n is the number of vertices in the original polygon
 */
// program to convert a given "polygon" into a set of convex polygons without introducing additional vertices
int main()  {
    for(int i = 1; i<=1; i++)  {
        local(i);

        auto start = chrono::steady_clock::now();
        // input format
        /*
        First line contains an integer n representing the number of vertices in the polygon.
        The next n lines contains vertices(3 things, i.e vertex_id x_coord y_coord)
        connected in a clockwise fashion. Coordinate and id both should be unique
        Eg:
        5
        A 0 0
        B 0 6
        C 3 3
        D 6 6
        E 6 0

        The edges are A-B-C-D-E-A
        */
        int numberOfVertices;
        fin>>numberOfVertices;
        vector<Vertex<T>*> vertices, verticesConst;
        vector<Edge<T>*> edges, edgesConst;
        //take input and store all the vertices and edges
        //Theta(n)

        for(int i = 0; i<numberOfVertices; i++)  {
            string id;
            pair<T,T> coordinate;
            // fin>>id>>coordinate.first>>coordinate.second;
            // id = "id"+to_string(i);
            fin>>id>>coordinate.first>>coordinate.second;
            // fout<<"Hello World\n";
            Edge<T>* edge = new Edge<T>();
            Vertex<T>* vertex = new Vertex<T>(id, coordinate, edge);
            edge->setOriginVertex(vertex);
            vertices.push_back(vertex);
            edges.push_back(edge);

            Edge<T>* edgeConst = new Edge<T>();
            Vertex<T>* vertexConst = new Vertex<T>(id, coordinate, edgeConst);
            edgeConst->setOriginVertex(vertexConst);
            verticesConst.push_back(vertexConst);
            edgesConst.push_back(edgeConst);

            //at least 2 vertices are there, so we can start making edge connections
            // not making the graph "directed" by splitting edges, since not required
            if(i>=1)  {
                edges[i-1]->setNextEdge(edges[i]);
                edges[i]->setPrevEdge(edges[i-1]);

                edgesConst[i-1]->setNextEdge(edgesConst[i]);
                edgesConst[i]->setPrevEdge(edgesConst[i-1]);

            }
            if(i==numberOfVertices-1)  {
                edges[i]->setNextEdge(edges[0]);
                edges[0]->setPrevEdge(edges[i]);

                edgesConst[i]->setNextEdge(edgesConst[0]);
                edgesConst[0]->setPrevEdge(edgesConst[i]);
            }
        }
        // saving the polygon in a dcel, and now operating on the vectors from above
        Face<T>* face = new Face<T>(1, edgesConst[0]);
        vector<Face<T>*> faces;
        faces.push_back(face);
        DCEL<T>* originalPolygon = new DCEL<T>(verticesConst, edgesConst, faces);

        DCEL<T>* polygonDecomposition = NULL;
        set<DCEL<T>*> minimalPolygonDecomposition;

        DCEL<T>* partitionPolygon = NULL;
        DCEL<T>* firstPolygonPartition = NULL;
        bool isFirstPolygonPartition = true;

        fout<<"Original Polygon:\n";
        printPolygon(edgesConst[0]);

        int minimalCountPartition = inf;
        int s = 0;
        // numberOfVertices=1;
        while(s<numberOfVertices)  {
            int countPartition = 1;
            do  {
                for(int i=0+s;i<vertices.size()+s;i++){
                    partitionPolygon = returnPartitionAndUpdatePolygon(vertices, edges, i, countPartition);
                    if(partitionPolygon)
                        break;
                }
                if(isFirstPolygonPartition)  {
                    auto firstVertices = vertices;
                    auto firstEdges = edges;
                    deepCopy(firstVertices, firstEdges, partitionPolygon->getVertices(), partitionPolygon->getEdges());
                    firstPolygonPartition = new DCEL<T>(firstVertices, firstEdges);
                    isFirstPolygonPartition=false;
                }
                if(partitionPolygon)  {
                    fout<<"Partition "<<countPartition<<":\n";
                    printPolygon(partitionPolygon->getEdges()[0]);
                }
                polygonDecomposition = mergePartitionWithDecomposition(partitionPolygon, polygonDecomposition, countPartition);
                if(partitionPolygon)
                    printDecompositon(polygonDecomposition);
                countPartition++;
            }
            while(partitionPolygon && vertices.size()!=0); // i.e until a null pointer is returned
            //fout<<"898\n";
            if(countPartition<minimalCountPartition)  {
                minimalPolygonDecomposition.clear();
                minimalPolygonDecomposition.insert(polygonDecomposition);
                minimalCountPartition=countPartition;
            }
            else if(countPartition==minimalCountPartition){
                minimalPolygonDecomposition.insert(polygonDecomposition);
            }
            isFirstPolygonPartition=true;
            polygonDecomposition=NULL;
            deepCopy(vertices, edges, verticesConst, edgesConst);
            s++;
            bool newFirstPolygon = false;
            //fout<<"910\n";
            while(!newFirstPolygon && s<numberOfVertices)  {
                //generate first polygon
                for(int i=0;i<vertices.size();i++){
                    partitionPolygon = returnPartitionAndUpdatePolygon(vertices, edges, i+s, 1);
                    if(partitionPolygon)
                        break;
                }
                if(partitionPolygon)  {
                    fout<<"First Partition only"<<":\n";
                    printPolygon(partitionPolygon->getEdges()[0]);
                }
                //fout<<"922\n";
                //fout.flush();
                if(polygonCompare(partitionPolygon, firstPolygonPartition))
                    s++;
                else
                    newFirstPolygon=true;
                deepCopy(vertices, edges, verticesConst, edgesConst);
            }
            //fout<<"928\n";
            //fout.flush();
        }
        //fout<<"932\n";
        //fout.flush();
        //output format
        /*
        construct a "graph" using a typical implementation to be passed to the visualiser
        script(along with the original polygon) using the exec call, resulting in a visual representation of the original polygon
        and the decomposition
        */

        // vector<Point> vertexListOriginal;
        // vector<vector<int>> adjacencyMatrixOriginal;
        // convertDCELtoGraph(originalPolygon, adjacencyMatrixOriginal, vertexListOriginal);
        // printGraph(vertexListOriginal, adjacencyMatrixOriginal);

        vector<Point> vertexList;
        vector<vector<int>> adjacencyMatrix;
        convertDCELtoGraph(*minimalPolygonDecomposition.begin(), adjacencyMatrix, vertexList);
        printGraph(vertexList, adjacencyMatrix);
        // fin.close();
        // fout.close();
        // fout.open("../Outputs/TvsN.txt", ios_base::app);

        auto stop = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        fout <<"\nNumber of Vertices: "<<numberOfVertices<< "\tTime taken:  "<< duration << " microseconds\n";
        // fout <<numberOfVertices<< "\t"<< duration << "\n";

        fin.close();
        fout.close();
        //visualise(originalPolygon->getVertices(), adjacencyMatrixOriginal, i);
        visualise((*minimalPolygonDecomposition.begin())->getVertices(), adjacencyMatrix, i);

        // runPythonScript(i);
    }

    return 0;
}