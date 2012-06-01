// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_SPANNING_TREE_PRIM_HPP
#define ORIGIN_GRAPH_SPANNING_TREE_PRIM_HPP

// FIXMEs
//  - need to fix vertex.hpp, or just make own iterator
//  - move nil_vertex to vertex.hpp

#include <limits>

#include <origin/graph/traits.hpp>
#include <origin/graph/label.hpp>



/*============================================================================*/
//template <typename T>  using Label_range = 

namespace origin
{
  enum class tri_color : char { black, gray, white };

  template <typename T>
    constexpr T max_val() { return std::numeric_limits<T>::max(); }

  // The Nil Vertex
    // FIXME move this into vertex.hpp
  template <typename V>
    V nil_vertex()
    { return V(max_val<V>()); }


/*============================================================================*/

  template <typename G, typename Edge_weight>
    labeling <Vertex<G>,Vertex<G>>
    prim(const G& g, Vertex<G> s, Edge_weight & w)
    {
      auto pred = label_vertices (g, nil_vertex<Vertex<G>>());
      auto color = label_vertices (g, tri_color::white);
      auto v_w = label_vertices (g,max_val<Value_type<Edge_weight>>());

      auto q = make_weight_queue(v_w);
      q.push(s);
      pred(s) = s;
      color(s) = tri_color::gray;

      while (!q.empty()) {
        Vertex<G> u = q.top();
        q.pop();
        for (auto e : incident_edges(g,u)) {
          Vertex<G> v = opposite(e,u);
          if ((color(v) != tri_color::black) && (w(e) < v_w(v))) {
            v_w(v) = w(e);
            pred(v) = u;
            if (color(v) == tri_color::white) {
              q.push(v);
              color(v) = tri_color::gray;
            } else if (color(v) == tri_color::gray) {
              q.update(v);
            }
          }
        }
        color(u) = tri_color::black;
      }
      return pred;
    }


// depricated code
#if 0
  template <typename G, typename Edge_weight>
    labeling <typename G::vertex,typename G::vertex>
    prim(const G& g, typename G::vertex s, Edge_weight & w)
    {
      using Vertex = typename G::vertex;
      using Edge = typename G::edge;
      /*using weight_type = typename Edge_weight::value_type;
      using weight_compare = label_compare<
        Vertex, labeling<Vertex, weight_type>, std::greater<weight_type>>;*/
      // make pred map, color map, vertex weight
      auto pred = label_vertices (g, Vertex(99999));
      auto color = label_vertices (g, tri_color::white);
      auto v_w = label_vertices (g,max_val<Value_type<Edge_weight>>());
      
      // Prepare the queue and start with the source vertex s
      /*mutable_binary_heap<Vertex, weight_compare> q (
        weight_compare(v_w, std::greater<weight_type>())
        );*/
      auto q = make_weight_queue(v_w);
      q.push(s);
      pred(s) = s;
      color(s) = tri_color::gray;

      while (!q.empty()) {
        Vertex u = q.top();
        q.pop();
        for (auto e_ : g.incident_edges(u)) {
          Edge e =*e_;
          Vertex v = opposite(e,u);
          if ((color(v) != tri_color::black) && (w(e) < v_w(v))) {
            v_w(v) = w(e);
            pred(v) = u;
            if (color(v) == tri_color::white) {
              q.push(v);
              color(v) = tri_color::gray;
            } else if (color(v) == tri_color::gray) {
              q.update(v);
            }
          }
        }
        color(u) = tri_color::black;
      }
      return pred;
    }
#endif

} // namespace origin

#endif //ORIGIN_GRAPH_SPANNING_TREE_PRIM_HPP