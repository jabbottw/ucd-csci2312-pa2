#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H

#include "Point.h"

namespace Clustering {

    typedef Point *PointPtr;
    typedef struct LNode *LNodePtr;

    struct LNode {
        PointPtr p;
        LNodePtr next;
        LNode(PointPtr pt, LNodePtr n) : p(pt), next(n) {}
    };

    class Cluster {
        unsigned int __dimensionality;
        int __size;
        LNodePtr __points;
        bool __release_points;
        bool __greedy;
        unsigned int __id;
        static unsigned int __idGenerator;

        void __del();
        void __cpy(LNodePtr pts);
        bool __in(const PointPtr ptr) const;

        class Centroid {
            unsigned int __dimensions;
            Point __p;
            bool __valid;

        public:
            Centroid(unsigned int d) :
                    __dimensions(d),
                    __p(d),
                    __valid(false)
            {}
            Centroid(const Centroid &cent) :
                    __dimensions(cent.__dimensions),
                    __p(cent.__p),
                    __valid(cent.__valid)
            {}
            const Point get() const { return __p; }
            void set(const Point &p) { __p = p; __valid = true; }
            bool isValid() const { return __valid; }
            void invalidate() { __valid = false; }
            void toInfinity();
        } __centroid;

    public:
        static const char POINT_CLUSTER_ID_DELIM;

        Cluster(unsigned int d) :
                Cluster (d, true, false) {};
        Cluster(unsigned int d, bool releasePts) :
                Cluster(d, releasePts, false) {};
        Cluster(unsigned int d, bool releasePts, bool gdy) :
                __dimensionality(d),
                __centroid(d),
                __size(0),
                __points(nullptr),
                __release_points(releasePts),
                __greedy(gdy),
                __id(__idGenerator++)
        {};

        // The big three: cpy ctor, overloaded operator=, dtor
        Cluster(const Cluster &);
        Cluster &operator=(const Cluster &);
        ~Cluster();

        // Getters
        int getSize() const { return __size; } // not in the requirements
        int getDimensionality() const { return __dimensionality; }
        unsigned int getId() const { return __id; }
        bool contains(const PointPtr &ptr) const;

        // Add/remove: They allow calling c1.add(c2.remove(p));
        void add(const PointPtr &);
        const PointPtr &remove(const PointPtr &);

        // Centroid functions
        void setCentroid(const Point &p) { __centroid.set(p); }
        // getCentroid doesn't check for validity, it's up to the user
        const Point getCentroid() const { return __centroid.get(); }
        void computeCentroid();
        bool isCentroidValid() const { return __centroid.isValid(); }
        void invalidateCentroid() { __centroid.invalidate(); }
        void pickPoints(unsigned int k, PointPtr *pointArray); // pick k initial centroids

        // Scoring functions
        double intraClusterDistance() const;
        int getClusterEdges() const;

        friend double interClusterDistance(const Cluster &c1, const Cluster &c2);
        friend int interClusterEdges(const Cluster &c1, const Cluster &c2);

        // Overloaded operators

        // Element access (aka direct access, array operator)
        const PointPtr &operator[](unsigned int u) const; // const version only

        // IO
        // - Friends
        friend std::ostream &operator<<(std::ostream &, const Cluster &);
        friend std::istream &operator>>(std::istream &, Cluster &);

        // - Friends
        friend bool operator==(const Cluster &lhs, const Cluster &rhs);
        friend bool operator!=(const Cluster &lhs, const Cluster &rhs);

        // Operators with Point-s
        // - Members
        Cluster &operator+=(const Point &rhs); // allocate point
        Cluster &operator-=(const Point &rhs); // delete point(s) (greedy=false by default)

        // Operators with Cluster-s
        // - Members
        Cluster &operator+=(const Cluster &rhs); // union
        Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

        // Binary operators for Cluster-s
        // - Friends
        friend const Cluster operator+(const Cluster &lhs, const Cluster &rhs);
        friend const Cluster operator-(const Cluster &lhs, const Cluster &rhs);

        // Binary operators for Cluster-s and Point pointers
        // - Friends
        friend const Cluster operator+(const Cluster &lhs, const PointPtr &rhs);
        friend const Cluster operator-(const Cluster &lhs, const PointPtr &rhs);

        class Move {
            PointPtr __p;
            Cluster *__from, *__to;
        public:
            Move(const PointPtr &p, Cluster *from, Cluster *to) :
                    __p(p), __from(from), __to(to)
            {}
            void perform() { if (__from != __to) __to->add(__from->remove(__p)); }
        };
    };

}
#endif //CLUSTERING_CLUSTER_H
