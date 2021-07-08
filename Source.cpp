#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

//Initial Values of Entry flow
double qIn = 100;  
double qIn2 = 50; 
double qIn3 = 10;

//Initial values of instantaneous Height
double h1 = 50;  
double h2 = 30;
double h3 = 10;


//Initial values of controllable references
double hc1 = 50;
double hc2 = 30;
double hc3 = 10;



// The function we want to execute on the new thread.
double dinamic_equation1(double h1, double t) {  // h1 = instantaneous height value  /  t = time

    //tank 1 properties
    double H = 2.5;
    double rzin = 5;
    double rzao = 10;
    double pi = 3.14;

    double h1Dot;
    double num;
    double den;
    double aux;
    double qOut;   //gama raiz de hi(t)

    qOut = qout(1, h1);

    num = (qIn - qOut) - qIn2;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h1;
    den = pow(den, 2);
    h1Dot = num / den;

    std::cout << h1Dot << "\n";

    return h1Dot;
}

double dinamic_equation2(double h2, double t) {  // h2 = instantaneous height value  /  t = time

    //tank 2 properties
    double H = 2.5;
    double rzin = 5;
    double rzao = 10;
    double pi = 3.14;

    double qOut;   //gama raiz de hi(t)
    double h2Dot;
    double num;
    double den;
    double aux;

    qOut = qout(2, h2);

    num = (qIn2 - qOut) - qIn3;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h2;
    den = pow(den, 2);
    h2Dot = num / den;

    std::cout << h2Dot << "\n";

    return h2Dot;
}

double dinamic_equation3(double h3, double t) {  // h3 = instantaneous height value  /  t = time

    //tank 3 properties
    double H = 2.5;
    double rzin = 5;
    double rzao = 10;
    double pi = 3.14;

    double qOut;   //gama raiz de hi(t)
    double h3Dot;
    double num;
    double den;
    double aux;

    qOut = qout(3, h3);

    num = qIn3 - qOut;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h3;
    den = pow(den, 2);
    h3Dot = num / den;

    std::cout << h3Dot << "\n";

    return h3Dot;
}

double runge_kutta(double xn, int n) {  // xn = calculation point  /  n = number of steps

    double x0, y0, h, yn, k1, k2, k3, k4, k;
    int i;

    x0 = 2; // seed value
    y0 = 5; //seed value

    /* Calculating step size (h) */
    h = (xn - x0) / n;

    /* Runge Kutta Method */
    cout << "\nx0\ty0\tyn\n";
    cout << "------------------\n";
    for (i = 0; i < n; i++)
    {
        k1 = h * (dinamic_equation1(x0, y0));
        k2 = h * (dinamic_equation1((x0 + h / 2), (y0 + k1 / 2)));
        k3 = h * (dinamic_equation1((x0 + h / 2), (y0 + k2 / 2)));
        k4 = h * (dinamic_equation1((x0 + h), (y0 + k3)));
        k = (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        yn = y0 + k;
        cout << x0 << "\t" << y0 << "\t" << yn << endl;
        x0 = x0 + h;
        y0 = yn;
    }

    /* Displaying result */
    cout << "\nValue of y at x = " << xn << " is " << yn;

    return yn;
}

double qout(int tank, double h) {
    
    double qo;
    double gama;

    if (tank == 1) {
        h = h1;
        gama = 3;
    }
    else if (tank == 2) {
        h = h2;
        gama = 2;
    }
    else {
        h = h3;
        gama = 1;
    }

    qo = gama * sqrt(h);

    return qo;

}

void proc_thread_1(string msg) {

    std::cout << "task1 says: " << msg << "\n";

    double resp = dinamic_equation1(10, 50);
    double test = runge_kutta(20, 3);

    std::cout << resp << "\n" << "resp1..." << "\n";
    std::cout << test << "\n" << "resp2..." << "\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void proc_thread_2(string msg) {

    std::cout << "task2 says: " << msg << "\n";

    double resp = dinamic_equation2(10, 50);
    double test = runge_kutta(20, 3);

    std::cout << resp << "\n" << "resp1..." << "\n";
    std::cout << test << "\n" << "resp2..." << "\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


void proc_thread_3(string msg) {

    std::cout << "task3 says: " << msg << "\n";

    double resp = dinamic_equation3(10, 50);
    double test = runge_kutta(20, 3);

    std::cout << resp << "\n" << "resp1..." << "\n";
    std::cout << test << "\n" << "resp2..." << "\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {


    thread t1(proc_thread_1, "Hello");
    thread t2(proc_thread_2, "YOUR");
    thread t3(proc_thread_3, "PUSSY");

    t1.join();
    t2.join();
    t3.join();

}