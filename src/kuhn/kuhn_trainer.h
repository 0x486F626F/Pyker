#ifndef PYKER_KUHN_TRAINER_H
#define PYKER_KUHN_TRAINER_H


#include "node.h"

#include <map>
#include <random>


class KuhnTrainer {
public:
    void train(int num_iterations);
private:
    std::mt19937 rng = std::mt19937(std::random_device()());
    std::map<InformationSet, Node> node_map;

    void shuffle(Deck& deck);
    double cfr(Deck cards, std::string history, double p0, double p1);
    /// Gets the node for the given information set.
    /// The node is created if it doesn't exist yet.
    std::map<InformationSet, Node>::iterator get_node(const InformationSet& info_set);
};


#endif //PYKER_KUHN_TRAINER_H
