#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

double qIn = 50;    //, qIn2, qIn3;
double h1 = 50;    //, h2, h3;
double qNext1 = 10;   //


// The function we want to execute on the new thread.
double dinamic_equation1(double h1, double t) {

    double H = 2.5;
    double rzin = 5;
    double rzao = 10;
    double qOut = 5;   //gama raiz de hi(t)
    double pi = 3.14;

    double h1Dot;
    double num;
    double den;
    double aux;

    num = (qIn - qOut) - qNext1;
    aux = (rzao - rzin) / H;
    den = pi * (rzin + aux);
    den = den * h1;
    den = pow(den, 2);
    h1Dot = num / den;

    std::cout << h1Dot << "\n";

    return h1Dot;
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



void proc_thread_1(string msg)
{

    std::cout << "task1 says: " << msg << "\n";

    double resp = dinamic_equation1(10, 50);
    double test = runge_kutta(20, 3);

    std::cout << resp << "\n" << "resp1..." << "\n";
    std::cout << test << "\n" << "resp2..." << "\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {

    // Constructs the new thread and runs it. Does not block execution.

    thread t1(proc_thread_1, "Hello");
    //thread tank2(tank_task);
    //thread tank3(tank_task);

    // Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
   t1.join();

}