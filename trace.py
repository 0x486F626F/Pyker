class Trace(object):
    def __init__(self, stage_num, balances, userpool, sidepools):
        self.stage_num = stage_num
        self.balances = balances
        self.userpool = userpool
        self.sidepools = sidepools
        self.pub_cards = []
        self.bets = []

    def add_bet(self, idx, amount):
        self.bets.append([idx, amount])

    def add_pub_cards(self, cards):
        self.pub_cards += cards

    def debug(self):
        print(self.userpool)
