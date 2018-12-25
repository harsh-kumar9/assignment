# Stock Order Execution

class CSVReader: Used to read the csv file.
CSVReader.get_data() - this function takes each row of the csv file as a string and then passes the string to CSVReader.split() to split the strings, removing the ',' from the whole string.

class Stock: This contains the details of the stock such as company_name, id, type(buy/sell) and has helper functions to get and set values.

class Order: Each object of the class inherits from the STOCK class and has it's 
own variables, state(0-closed,1-open).
Order.execute_order() - called by the previous stock order and passes the opposite order as parameter and then calculates the remaining stock quantity for both the orders and then updates it.

class OrderList: class that contains an array of Orders(each row of the csv file),
OrderList.Execute_all_orders() - Two loops are run:
   loop i: 0 -> end of list:
     if order i is open
       loop j:i+1 -> end of list
          if order j is open and company j==company i and type j opposite of type i:
             order i.execute_order(order j)
             // At this point, the remaining quantity of all the orders are updated
            // but when the Print_all_orders() function is called, it displays the                        // original values.

OrderList.Print_all_orders() - Prints all the orders to the screen with remaining quantity.

