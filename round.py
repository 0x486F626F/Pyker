import deck
import hand
import trace


class Round(object):
    """
    One round of the game.
    Assume that len(players) >= 2 and the first player is the small blind
    """

    def __init__(self, blind, balances, players):
        self.blind = blind
        self.balances = balances
        self.players = players

        self.deck = deck.Deck()
        self.n_player = len(players)
        self.active_players = [i for i in range(self.n_player)]
        self.folded = [False for _ in range(self.n_player)]

        self.user_pool = [0 for _ in range(self.n_player)]
        self.side_pools = []
        self.cards = [[] for _ in range(self.n_player)]

        self.trace = []

        for i in range(self.n_player):
            players[i].reset(i)

    def _player_actions(self, queue, log):
        cur = 0
        while cur < len(queue):
            idx = queue[cur]
            cur += 1
            _debug("")
            _debug('Player ' + str(idx))
            _debug('Current: ' + str(cur) + ' of Queue: ' + str(queue))
            _debug('Balances: ' + str(self.balances))
            _debug('User Pool: ' + str(self.user_pool))

            low = min(self.balances[idx], max(self.user_pool)) - self.user_pool[idx]
            high = min(self.balances[idx], _rank2(self.balances)) - self.user_pool[idx]

            if high == 0:
                continue

            _debug('Range: ' + str([low, high]))
            bet = self.players[idx].action([low, high], log)
            assert(bet < 0 or low <= bet <= high)

            if bet < 0:
                self.active_players.remove(idx)
                self.folded[idx] = False
                continue

            log.add_bet(idx, bet)
            self.user_pool[idx] += bet

            if self.balances[idx] == self.user_pool[idx]:  # all-in
                _debug('Player %d All in' % idx)
                # self.active_players.remove(idx)
                continue

            if bet > low:  # raise
                # if current player raised, extend the queue to let all active
                # players to make decision again
                l = self.active_players.index(idx)
                subsequent = set(queue[cur:])
                players = self.active_players[l+1:] + self.active_players[:l]
                for each in players:
                    if each not in subsequent:
                        queue.append(each)

        _debug("")
        _debug('Pool: ' + str(self.user_pool))
        _debug('Active Players: ' + str(self.active_players))

        # check side pool
        active_player_pool = [self.user_pool[i] for i in self.active_players]
        min_bet = min(active_player_pool)
        new_pool = []
        for idx in range(self.n_player):
            if not self.folded[idx] and self.user_pool[idx] > min_bet:
                new_pool.append(self.user_pool[idx] - min_bet)
                self.user_pool[idx] = min_bet
            else:
                new_pool.append(0)
            self.balances[idx] -= self.user_pool[idx]
        self.side_pools.append([self.user_pool[:], self.active_players[:]])
        self.user_pool = new_pool
        # remove all-in players
        for idx in range(self.n_player):
            if self.balances[idx] == 0 and idx in self.active_players:
                self.active_players.remove(idx)

        _debug('Side pool: ' + str(self.side_pools[-1][0]))
        _debug('Participants: ' + str(self.side_pools[-1][1]))
        _debug('New pool: ' + str(self.user_pool))
        _debug('New Active: ' + str(self.active_players))

    def _stage0(self):
        _debug('Stage 0')
        log = trace.Trace(len(self.trace), self.balances, self.user_pool, self.side_pools)

        # Small blind and big blind
        for i in range(2):
            bet = self.players[i].blind(self.blind * (i + 1))
            assert(bet >= self.blind)
            self.user_pool[i] += bet
            log.add_bet(i, bet)

        # deal 2 cards
        for _ in range(2):
            for i in self.active_players:
                card = self.deck.deal()
                self.cards[i].append(card)
                self.players[i].deal(card)

        player_queue = self.active_players[2:] + self.active_players[:2]
        if len(player_queue) > 1:
            self._player_actions(player_queue, log)
        self.trace.append(log)

    def _stage1(self):
        _debug('Stage 1')
        log = trace.Trace(len(self.trace), self.balances, self.user_pool, self.side_pools)

        # burn a card
        self.deck.deal()

        flop = [self.deck.deal() for _ in range(3)]
        log.add_pub_cards(flop)
        for player in self.players:
            for card in flop:
                player.deal(card)
        for each in self.cards:
            each += flop

        player_queue = self.active_players[:]
        if len(player_queue) > 1:
            self._player_actions(player_queue, log)
        self.trace.append(log)

    def _stage2(self):
        _debug('Stage 2')
        log = trace.Trace(len(self.trace), self.balances, self.user_pool, self.side_pools)

        # burn a card
        self.deck.deal()

        card = self.deck.deal()
        log.add_pub_cards([card])
        for player in self.players:
            player.deal(card)
        for each in self.cards:
            each.append(card)

        player_queue = self.active_players[:]
        if len(player_queue) > 1:
            self._player_actions(player_queue, log)
        self.trace.append(log)

    def result(self):
        self._stage0()
        self._stage1()
        self._stage2()
        self._stage2()

        # add current user pool to side pools
        self.side_pools.append([self.user_pool[:], self.active_players[:]])
        for idx in range(self.n_player):
            self.balances[idx] -= self.user_pool[idx]

        for p in self.side_pools:
            _debug('Pool: ' + str(p[0]) + ' Players: ' + str(p[1]))
        _debug('Balances: ' + str(self.balances))

        for p in self.side_pools:
            pool = p[0]
            players = p[1]
            if len(players) == 1:
                self.balances[players[0]] += sum(pool)
                continue

            candidates = [[i, hand.Hand(self.cards[i])] for i in players]
            for candidate in candidates:
                _debug(candidate[0])
                candidate[1].print_hand()
            # use user id as secondary key so that lowest player id gets extra
            candidates = sorted(candidates, key=lambda x: (x[1], x[0]))
            winners = []
            for candidate in candidates:
                if candidate[1] == candidates[-1][1]:
                    winners.append(candidate[0])
            _debug('Winners: ' + str(winners))
            for i in winners:
                self.balances[i] += sum(pool) // len(winners)
            self.balances[i] += sum(pool) % len(winners)

        _debug('Balances: ' + str(self.balances))
        shown_hands = [[i, self.cards[i]] for i in range(self.n_player)
                       if not self.folded[i]]
        for idx in range(self.n_player):
            self.players[idx].set_new_balance(self.balances[i])
            self.players[idx].notify_shownhands(shown_hands)
        return self.balances


def _debug(info):
    print(info)


def _rank2(arr):
    first = second = min(arr)
    for each in arr:
        if each > first:
            second = first
            first = each
        elif each > second:
            second = each
    return second
