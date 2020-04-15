import deck
import player
import hand
import trace

def debug(info):
    print(info)

# One round of game, assume the len(players) >= 2 and the first player is the
# smal blind
class Round(object):
    def __init__(self, blind, balances, players):
        self.blind = blind
        self.balances = balances
        self.players = players

        self.deck = deck.Deck()
        self.nplayer = len(players)
        self.active_players = [i for i in range(self.nplayer)]
        self.folded = [False for i in range(self.nplayer)]
        
        self.userpool = [0 for i in range(self.nplayer)]
        self.sidepools = []
        self.cards = [[] for i in range(self.nplayer)]

        self.trace = []

        for i in range(self.nplayer):
            players[i].reset(i)

    def player_actions(self, queue, log):
        cur = 0
        while cur < len(queue):
            idx = queue[cur]
            cur += 1
            debug('Player ' + str(idx))
            debug('Current: ' + str(cur) + ' of Queue: ' + str(queue))
            debug('Balances: ' + str(self.balances))
            debug('User Pool: ' + str(self.userpool))

            low = min(self.balances[idx], max(self.userpool)) - self.userpool[idx]
            high = min(self.balances[idx], rank2(self.balances)) - self.userpool[idx]

            if high == 0 or len(self.active_players) == 1:
                continue

            debug('Range: ' + str([low, high]))
            bet = self.players[idx].action([low, high], log)
            assert(bet < 0 or (bet >= low and bet <= high))

            if bet < 0:
                self.active_players.remove(idx)
                self.folded[idx] = False
                continue

            log.add_bet(idx, bet)
            self.userpool[idx] += bet

            if self.balances[idx] == self.userpool[idx]:  # all-in
                debug('Player %d All in' % idx)
                #self.active_players.remove(idx)
                continue

            if bet > low:               # raise
                # if current player raised, extend the queue to let all active
                # players to make decision again
                l = self.active_players.index(idx)
                subsequent = set(queue[cur:])
                players = self.active_players[l+1:] + self.active_players[:l]
                for each in players:
                    if each not in subsequent:
                        queue.append(each)
        debug('Pool: ' + str(self.userpool))
        debug('Active Players: ' + str(self.active_players))

        # check side pool
        active_player_pool = [self.userpool[i] for i in self.active_players]
        min_bet = min(active_player_pool)
        newpool = []
        for idx in range(self.nplayer):
            if not self.folded[idx] and self.userpool[idx] > min_bet:
                newpool.append(self.userpool[idx] - min_bet)
                self.userpool[idx] = min_bet
            else:
                newpool.append(0)
            self.balances[idx] -= self.userpool[idx]
        self.sidepools.append([self.userpool[:], self.active_players[:]])
        self.userpool = newpool
        # remove all-in players
        for idx in range(self.nplayer):
            if self.balances[idx] == 0 and idx in self.active_players:
                self.active_players.remove(idx)

        debug('Sidepool: ' + str(self.sidepools[-1][0]))
        debug('Participants: ' + str(self.sidepools[-1][1]))
        debug('New pool: ' + str(self.userpool))
        debug('New Active: ' + str(self.active_players))


    def stage0(self):
        debug('Stage 0')
        log = trace.Trace(len(self.trace), self.balances, self.userpool, 
                self.sidepools)

        # Small blind and big blind 
        for i in range(2):
            bet = self.players[i].blind(self.blind * (i+1))
            assert(bet >= self.blind)
            self.userpool[i] += bet
            log.add_bet(i, bet)  

        # deal 2 cards
        for c in range(2):
            for i in self.active_players:
                card = self.deck.deal()
                self.cards[i].append(card)
                self.players[i].deal(card)

        player_queue = self.active_players[2:] + self.active_players[:2]
        if len(player_queue) > 1:
            self.player_actions(player_queue, log)
        self.trace.append(log)

    def stage1(self):
        debug('Stage 1')
        log = trace.Trace(len(self.trace), self.balances, self.userpool, 
                self.sidepools)
        burned = self.deck.deal()
        cards = [self.deck.deal() for i in range(3)]
        log.add_pub_cards(cards)
        for p in self.players:
            for c in cards:
                p.deal(c)
        for each in self.cards:
            each += cards

        player_queue = self.active_players[:]
        if len(player_queue) > 1:
            self.player_actions(player_queue, log)
        self.trace.append(log)

    def stage2(self):
        debug('Stage 2')
        log = trace.Trace(len(self.trace), self.balances, self.userpool, 
                self.sidepools)
        burned = self.deck.deal()
        card = self.deck.deal()
        log.add_pub_cards([card])
        for p in self.players:
            p.deal(card)
        for each in self.cards:
            each.append(card)

        player_queue = self.active_players[:]
        if len(player_queue) > 1:
            self.player_actions(player_queue, log)
        self.trace.append(log)


    def result(self):
        self.stage0()
        self.stage1()
        self.stage2()
        self.stage2()
        
        # add current user pool to sidepools
        self.sidepools.append([self.userpool[:], self.active_players[:]])
        for idx in range(self.nplayer):
            self.balances[idx] -= self.userpool[idx]

        for p in self.sidepools:
            debug('Pool: ' + str(p[0]) + ' Players: ' + str(p[1]))
        debug('Balances: ' + str(self.balances))


        for p in self.sidepools:
            pool = p[0]
            players = p[1]
            if len(players) == 1:
                self.balances[players[0]] += sum(pool)
                continue
            
            candidates = [[i, hand.Hand(self.cards[i])] for i in players]
            for c in candidates:
                debug(c[0])
                c[1].printhand()
            # use user id as secondary key so that lowest player id gets extra
            candidates = sorted(candidates, key=lambda x: (x[1], x[0]))
            winners = []
            for c in candidates:
                if c[1] == candidates[-1][1]:
                    winners.append(c[0])
            debug('Winners: ' + str(winners))
            for i in winners:
                self.balances[i] += sum(pool) // len(winners)
            self.balances[i] += sum(pool) % len(winners)

        debug('Balances: ' + str(self.balances))
        shownhands = [[i, self.cards[i]] for i in range(self.nplayer) 
                if not self.folded[i]]
        for idx in range(self.nplayer):
            self.players[idx].set_new_balance(self.balances[idx])
            self.players[idx].notify_shownhands(shownhands)
        return self.balances
            

class Game(object):
    def __init__(self, blind, balances, players):
        self.blind = blind
        self.balances = balances
        self.players = players

    def one_round(self):
        r = Round(self.blind, self.balances, self.players)
        self.balances = r.result()
        
        indexes = [i for i in range(len(self.balances)) 
                if self.balances[i] >= self.blind*2]
        self.balances = [self.balances[i] for i in indexes]
        self.players = [self.players[i] for i in indexes]

        # rotate small blind
        self.balances = self.balances[1:] + [self.balances[0]]
        self.players = self.players[1:] + [self.players[0]]

    def start(self):
        while len(self.players) > 1:
            self.one_round()


def rank2(arr):
    first = second = min(arr)
    for each in arr:
        if each > first:
            second = first
            first = each
        elif each > second:
            second = each
    return second

