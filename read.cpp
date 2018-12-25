#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

/*
This class represents the stock information of  a particular company.
*/
class Stock
{
  protected:
    int stock_id;
    string company;
    int quantity;

  public:
    Stock(int id, string name, int amount)
    {
        stock_id = id;
        company = name;
        quantity = amount;
    }
};

/*
This class contains all the information regarding each order,
it's type, status.
The execute_order
*/

class Order : protected Stock
{
  protected:
    int rem_quantity;
    string type; // 1 represents BUY and 0 represents SELL
    bool status; // 1 represents OPEN order and 0 represent Closed order

  public:
    Order(int id, string name, int amount, string in_type) : Stock(id, name, amount)
    {

        type = in_type;
        rem_quantity = amount;
        status = 1; // keeping all the orders open on initialization.
    }

    bool get_status()
    {
        return status;
    }

    void set_status()
    {
        status = !status;
    }

    string get_type()
    {
        return type;
    }

    int get_rem_quantity()
    {
        // cout << "getting rem quantity - " << rem_quantity << endl;
        return rem_quantity;
    }

    void set_rem_quantity(int qty)
    {
        // cout << "setting quantity - " << qty << endl;
        rem_quantity = qty;
        // cout << "setting quantity  setted- " << rem_quantity << endl;
    }

    string get_company_name()
    {
        return company;
    }

    int get_quantity()
    {
        return quantity;
    }

    int get_id()
    {
        return stock_id;
    }

    void execute_order(Order &new_order);
};

void Order::execute_order(Order &new_order)
{
    int prev_qty = get_rem_quantity();
    int new_qty = new_order.get_rem_quantity();
    if (prev_qty > new_qty)
    {

        set_rem_quantity(prev_qty - new_qty);
        new_order.set_rem_quantity(0);
        new_order.set_status();
    }
    else
    {
        new_order.set_rem_quantity(new_qty - prev_qty);
        set_rem_quantity(0);
        set_status();
    }
    cout << endl;
}

class OrderList
{

  protected:
    vector<Order> Order_list;

  public:
    OrderList(vector<Order> Orders)
    {
        Order_list = Orders;
    }
    vector<Order> get_Order_List()
    {
        return Order_list;
    }

    void Execute_all_orders();
    void Print_all_orders();
};

void OrderList::Execute_all_orders()
{
    // vector <Order> Order_list = get_Order_List();
    for (auto x = 0; x < Order_list.size(); x++)
    {
        cout << "Loop: " << x << endl;
        auto prev_order = Order_list[x];
        // check to see if the current order status is open
        // if open, then check the other
        if (prev_order.get_status())
        {
            for (auto y = x + 1; y < Order_list.size(); y++)
            {
                auto new_order = Order_list[y];
                if (new_order.get_status())
                {
                    if ( // check if the stock orders are from the same company and opposite sides.
                        new_order.get_company_name() == prev_order.get_company_name() && new_order.get_type() != prev_order.get_type())
                    {
                        prev_order.execute_order(new_order);
                        cout<<prev_order.get_id()<<" "<<prev_order.get_rem_quantity()<<endl;
                        cout<<new_order.get_id()<<" "<<new_order.get_rem_quantity()<<endl;
                        cout<<endl;
                    }
                }
            }
        }
    }
    cout<<"Id"<<"   "<<"Side"<<"    "<<"Company"<<"     "<<"Quantity"<<"    "<<"Remaining Qty"<<endl;
    for(Order order: Order_list){
        cout<<order.get_id()<<" "<<order.get_type()<<" "<<order.get_company_name()<<" "<<order.get_quantity()<<","<<order.get_rem_quantity()<<endl;
    }
    // Print_all_orders();
}

void OrderList::Print_all_orders()
{

    cout << "Id"
         << "   "
         << "Side"
         << "    "
         << "Company"
         << "     "
         << "Quantity"
         << "    "
         << "Remaining Qty" << endl;
    for (Order order : Order_list)
    {
        cout << order.get_id() << " " << order.get_type() << " " << order.get_company_name() << " " << order.get_quantity() << "," << order.get_rem_quantity() << endl;
    }
}

/*
This class is used to read csv files and then store the orders.
*/
class CSVReader
{

    string fileName, delimeter;

  public:
    CSVReader(string filename, string delm = ",") : fileName(filename), delimeter(delm)
    {
    }

    // function to get the data.
    vector<Order> getData();
    // function ot split each row using delimeter.
    vector<string> split(string s);
};

// split the row along the delimeter: ','
vector<string> CSVReader::split(string s)
{
    vector<string> tokens;
    string token;
    int i;
    s.append(",");
    for (i = 0; i < s.length(); i++)
    {
        // cout<<s[i]<<endl;
        if (s[i] != ',')
        {
            token.push_back(s[i]);
        }
        else
        {
            tokens.push_back(token);
            token = "";
        }
    }
    tokens.push_back(token);
    cout << endl;
    return tokens;
}

vector<Order> CSVReader::getData()
{
    ifstream file(fileName);

    vector<Order> List;

    string line = "";

    while (!file.eof())
    {

        getline(file, line);

        auto data = split(line);

        Order new_order(stoi(data[0]), data[2], stoi(data[3]), data[1]);
        List.push_back(new_order);
    }

    return List;
}

int main()
{
    CSVReader reader("stocks.csv");

    // contains all the rows of the csv file.
    // auto orders = reader.getData();

    OrderList Orders(reader.getData());

    // executing all the transactions.
    Orders.Execute_all_orders();

    return 0;
}
