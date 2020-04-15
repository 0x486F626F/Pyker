from .round import Round


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
            print('================================')
