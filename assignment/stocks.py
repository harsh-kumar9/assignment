import csv
import sys


class Stock:
    def __init__(self, list):
        self.s_id = list[0]
        self.side = list[1]

        # denoting Buy as 1 and Sell as 0
        if self.side == 'Buy':
            self.side = 1
        else:
            self.side = 0

        self.cmp_name = list[2]
        self.amt = int(list[3])
        self.status = "Open"
        self.balance = self.amt

    def __setattr__(self, name, value):
        self.__dict__[name] = value

    def print_stock(self):
        print(f"{self.s_id}, {self.side}, {self.cmp_name}, {self.amt}, {self.status}")



class Transactions:
    def __init__(self, stocks):
        self.transaction_list = stocks
        self.map = {}

    # calculate the balance amt of both the BUY and SELL order
    def transact(self, order1, order2):
        
        #print("hey transact")
        if order1.balance > order2.balance:
            order1.balance = order1.balance - order2.balance
            order2.balance = 0
        else:
            order2.balance = order2.balance - order1.balance
            order1.balance = 0

        return order1, order2

    def compute(self):

        
        for i in range(len(self.transaction_list)):
            
            order = self.transaction_list[i]
            
            # check if the side is BUY and the order is open, else continue..
            if order.side == 1 and order.status == 'Open':
                
                buy_key = order.cmp_name+str(order.side)
                sell_key = order.cmp_name+str(1-order.side)
                
                # check if there are no sell orders pending.
                if sell_key not in self.map:
                    #add the buy order to a list.
                    self.map[buy_key] = [order]

                elif len(self.map[sell_key]) == 0:

                    # check to see if there are any buy orders left,
                    #  if buy orders are left, add the new one to the list.
                    if buy_key in self.map:
                        self.map[buy_key].append(order)
                    else:
                        self.map[buy_key] = [order]
                
                # execute transaction if their is a sell order open
                else:
                    
                    # complete the orders(buy,sell) until closed
                    while(len(self.map[sell_key]) > 0):
                        
                        
                        order1 = self.map[sell_key][0]
                        # complete one transaction.
                        order1, order = self.transact(self.map[sell_key][0], order)
                        
                        # change status of order to closed if balance = 0.
                        if order1.balance == 0:
                            self.map[sell_key][0].status = "Closed"
                            # remove the closed order from the list.
                            del(self.map[sell_key][0])
                        
                        # if the current order has no balance, break from the loop
                        if order.balance == 0:
                            order.status = "Closed"
                            break 
                        
                    if buy_key not in self.map and order.balance > 0:
                        self.map[buy_key] = [order]


                
            if order.side == 0 and order.status == "Open":
    
                sell_key = order.cmp_name+str(order.side)
                buy_key = order.cmp_name+str(1+order.side)
                
                # check if there are no buy orders pending.
                if buy_key not in self.map:
                    # add the sell order to a list.
                    self.map[sell_key] = [order]
                
                elif len(self.map[buy_key]) == 0:

                    # check to see if there are any buy orders left,
                    #  if buy orders are left, add the new one to the list.
                    if sell_key in self.map:
                        self.map[sell_key].append(order)
                    else:
                        self.map[sell_key] = [order]

                
                # execute transaction if their is a buy order open
                else:
    
                    # complete the orders(buy,sell) until closed
                    while(len(self.map[buy_key]) > 0):
                        
                        order1 = self.map[buy_key][0]
                        # complete one transaction.
                        order1, order = self.transact(self.map[buy_key][0], order)
                        
                        # change status of order to closed if balance = 0.
                        if order1.balance == 0:
                            self.map[buy_key][0].status = "Closed"
                            # remove the closed order from the list.
                            del(self.map[buy_key][0])
                        
                        # if the current order has no balance, break from the loop
                        if order.balance == 0:
                            order.status = "Closed"
                            break 
                    
                    if sell_key not in self.map and order.balance > 0:
                        self.map[sell_key] = [order]
                      


    def print_stocks(self):
        for order in self.transaction_list:
            print(f"{order.s_id}  {order.side}  {order.cmp_name}   {order.amt},{order.balance}   {order.status}")
    

def main():
    
    # list of stocks
    stocks = []

    # get the values from the csv file.
    with open('stocks.csv', 'rt') as f:
        reader = csv.reader(f)
        stocks = [Stock(row) for row in reader]

    # create the list of transactions
    transactions = Transactions(stocks)

    # compute the transactions.
    transactions.compute()

    # print all the orders
    transactions.print_stocks() 
    

if __name__ == "__main__":

    sides = ['Sell','Buy']
    main()
