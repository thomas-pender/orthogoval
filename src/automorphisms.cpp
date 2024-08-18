# include <config.h>

# include <iostream>
# include <fstream>
# include <sstream>
# include <memory>
# include <cstdint>
# include <cstddef>
# include <unordered_set>
# include <list>
# include <vector>
# include <utility>

/* project headers */
# include <matrix.h>
# include <ag.h>

# define NGENS 2

/* hashing pairs */
namespace AG_pair
{
  struct hash_func
  {
    std::size_t operator()(std::pair<AG, matrix> const& p) const {
      return AG::hash_func()(p.first);
    }
  };
  struct comp_func
  {
    bool operator()(std::pair<AG, matrix> const& p1,
                    std::pair<AG, matrix> const& p2) const {
      return p1.first == p2.first;
    }
  };
}

/* driver */
int main(int argc, char **argv)
{
  if ( argc < 5 ) {
    std::cerr << "USAGE -- requires <degree> <order> <generators> <spread>\n";
    return 1;
  }

  std::pair<AG, matrix> canonical;

  /* read in generators of matrix group */
  std::vector<matrix> gens(NGENS);
  {
    std::size_t n;
    std::stringstream ss(argv[1]);
    ss >> n;

    std::fstream F;
    F.open(argv[3], std::ios::in);

    std::for_each(gens.begin(), gens.end(), [&](auto &x){
      x.resize(n);
      F >> x;
    });

    F.close();

    canonical.second = identity_matrix32(n);
  }

  /* read in canonical spread */
  {
    AG can{};

    std::size_t order;
    std::stringstream ss(argv[2]);
    ss >> order;

    std::fstream F;
    F.open(argv[4], std::ios::in);

    can.resize(order+1, order);
    F >> can;

    F.close();

    canonical.first = can;
  }

  /* calculate intersection of orbit and orthogoval pairs */
  std::size_t nvertices{0};
  std::unordered_set<std::pair<AG, matrix>, AG_pair::hash_func, AG_pair::comp_func> orbit{canonical};
  std::list<std::pair<AG, matrix> > prev_first{canonical};

  std::fstream trans, auts, verts;
  trans.open("transversal.txt", std::ios::out);
  auts.open("automorphisms.txt", std::ios::out);
  verts.open("vertices.txt", std::ios::out);

  do {
    std::list<std::pair<AG, matrix> > prev_second{};
    for ( const auto &p : prev_first )
      for ( const auto &gen : gens ) {
        std::pair<AG, matrix> new_p{gen * p.first, gen * p.second};
        auto it = orbit.find(new_p);
        if ( it == orbit.end() ) {
          if ( orthogoval(canonical.first, new_p.first) ) {
            verts << new_p.first << '\n' << std::flush;
            new_p.second.print(trans);
            trans << '\n' << std::flush;
            nvertices++;
          }
          orbit.insert(new_p);
          prev_second.push_back(std::move(new_p));
        }
        else {
          matrix aut_gen = (*it).second.inverse() * gen * p.second;
          if ( aut_gen != canonical.second ) {
            aut_gen.print(auts);
            auts << '\n' << std::flush;
          }
        }
      }
    prev_first = std::move(prev_second);
  } while( !prev_first.empty() );

  verts << "number of vertices = " << nvertices << '\n';

  return 0;
}
