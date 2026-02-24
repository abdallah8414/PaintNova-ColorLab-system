#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================= USERS ================= */

struct User
{
    int id;
    char username[30];
    char password[30];
    int role;   // 1 Worker 2 Finance 3 Admin
};

struct Product
{
    int id;
    char name[50];
    char category[30];
    int stock;
    float price;
    struct Product *next;
};

struct CustomerOrder
{
    int id;
    char customerName[50];
    char phone[20];
    char productName[50];
    int quantity;
    int priority;
    int status;   // 0 Pending 1 Approved 2 Paid -1 Rejected
    float price;
    struct CustomerOrder *next;
};

/* ================= GLOBAL ================= */

struct User users[50];
int userCount=0;

struct User *currentUser=NULL;

struct Product *productHead=NULL;
struct CustomerOrder *orderHead=NULL;

int userCounter=1;
int productCounter=1001;
int orderCounter=1;

/* ================= ADMIN ================= */

void initializeAdmin()
{
    users[0].id=userCounter++;
    strcpy(users[0].username,"admin");
    strcpy(users[0].password,"admin123");
    users[0].role=3;
    userCount=1;
}

/* ================= LOGIN ================= */

int login()
{
    char u[30],p[30];

    printf("Username: ");
    scanf("%s",u);

    printf("Password: ");
    scanf("%s",p);

    for(int i=0;i<userCount;i++)
    {
        if(!strcmp(users[i].username,u) &&
           !strcmp(users[i].password,p))
        {
            currentUser=&users[i];
            return 1;
        }
    }

    printf("Login Failed\n");
    return 0;
}

/* ================= USER ================= */

void registerUser()
{
    struct User u;

    u.id=userCounter++;

    printf("Username: ");
    scanf("%s",u.username);

    printf("Password: ");
    scanf("%s",u.password);

    printf("Role (1 Worker 2 Finance 3 Admin): ");
    scanf("%d",&u.role);

    users[userCount++]=u;

    printf("User Registered ID=%d\n",u.id);
}

/* ================= PRODUCT ================= */

void addProduct()
{
    struct Product *p=malloc(sizeof(struct Product));

    p->id=productCounter++;

    printf("Product Name: ");
    scanf("%s",p->name);

    printf("Category: ");
    scanf("%s",p->category);

    printf("Stock: ");
    scanf("%d",&p->stock);

    printf("Price: ");
    scanf("%f",&p->price);

    p->next=productHead;
    productHead=p;

    printf("Product Added ID=%d\n",p->id);
}

struct Product *findProduct(char name[])
{
    struct Product *t=productHead;

    while(t)
    {
        if(!strcmp(t->name,name))
            return t;

        t=t->next;
    }

    return NULL;
}

void viewProducts()
{
    struct Product *t=productHead;

    if(!t)
    {
        printf("No Products\n");
        return;
    }

    while(t)
    {
        printf("Name:%s Category:%s Stock:%d Price:%.2f\n",
               t->name,t->category,t->stock,t->price);

        t=t->next;
    }
}

/* ================= WORKER ================= */

void addCustomerOrder()
{
    struct CustomerOrder *o=
        malloc(sizeof(struct CustomerOrder));

    o->id=orderCounter++;

    printf("Customer Name: ");
    scanf("%s",o->customerName);

    printf("Phone: ");
    scanf("%s",o->phone);

    printf("Product Name: ");
    scanf("%s",o->productName);

    struct Product *p=
        findProduct(o->productName);

    if(!p)
    {
        printf("Product Not Found\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d",&o->quantity);

    if(p->stock < o->quantity)
    {
        printf("Stock Safety Protection\n");
        return;
    }

    printf("Priority (1 VIP 2 Normal): ");
    scanf("%d",&o->priority);

    o->price=p->price;
    o->status=0;

    o->next=orderHead;
    orderHead=o;

    printf("Order Sent To Finance ID=%d\n",o->id);
}

void workerViewOrders()
{
    struct CustomerOrder *t=orderHead;

    if(!t)
    {
        printf("No Orders\n");
        return;
    }

    while(t)
    {
        printf("OrderID:%d Customer:%s Product:%s Qty:%d Status:",
               t->id,
               t->customerName,
               t->productName,
               t->quantity);

        if(t->status==0) printf("Pending");
        else if(t->status==1) printf("Approved");
        else if(t->status==2) printf("Paid");
        else if(t->status==-1) printf("Rejected");

        printf("\n");

        t=t->next;
    }
}

/* ================= FINANCE ================= */

void approveOrder()
{
    int id;

    printf("Order ID: ");
    scanf("%d",&id);

    struct CustomerOrder *t=orderHead;

    while(t)
    {
        if(t->id==id && t->status==0)
        {
            t->status=1;
            printf("Order Approved\n");
            return;
        }

        t=t->next;
    }

    printf("Order Not Found\n");
}

void rejectOrder()
{
    int id;

    printf("Order ID: ");
    scanf("%d",&id);

    struct CustomerOrder *t=orderHead;

    while(t)
    {
        if(t->id==id && t->status==0)
        {
            t->status=-1;
            printf("Order Rejected\n");
            return;
        }

        t=t->next;
    }
}

void makePayment()
{
    int id;

    printf("Order ID: ");
    scanf("%d",&id);

    struct CustomerOrder *t=orderHead;

    while(t)
    {
        if(t->id==id && t->status==1)
        {
            t->status=2;
            printf("Payment Pipeline Completed\n");
            return;
        }

        t=t->next;
    }

    printf("Payment Failed\n");
}

void viewOrders()
{
    workerViewOrders();
}

/* ================= MAIN ================= */

int main()
{
    initializeAdmin();

    int choice;

    while(1)
    {
        printf("\n1.Login 2.Exit\nChoice: ");
        scanf("%d",&choice);

        if(choice==2) exit(0);

        if(!login()) continue;

        /* ADMIN */

        if(currentUser->role==3)
        {
            while(1)
            {
                printf("\nADMIN MENU\n");
                printf("1.Register User\n");
                printf("2.Add Product\n");
                printf("3.View Products\n");
                printf("4.Logout\nChoice: ");

                scanf("%d",&choice);

                if(choice==4) break;

                if(choice==1) registerUser();
                if(choice==2) addProduct();
                if(choice==3) viewProducts();
            }
        }

        /* WORKER */

        else if(currentUser->role==1)
        {
            while(1)
            {
                printf("\nWORKER MENU\n");
                printf("1.Add Customer Order\n");
                printf("2.View Orders\n");
                printf("3.View Products\n");
                printf("4.Logout\nChoice: ");

                scanf("%d",&choice);

                if(choice==4) break;

                if(choice==1) addCustomerOrder();
                if(choice==2) workerViewOrders();
                if(choice==3) viewProducts();
            }
        }

        /* FINANCE */

        else if(currentUser->role==2)
        {
            while(1)
            {
                printf("\nFINANCE MENU\n");
                printf("1.View Orders\n");
                printf("2.Approve Order\n");
                printf("3.Reject Order\n");
                printf("4.Make Payment\n");
                printf("5.Logout\nChoice: ");

                scanf("%d",&choice);

                if(choice==5) break;

                if(choice==1) viewOrders();
                if(choice==2) approveOrder();
                if(choice==3) rejectOrder();
                if(choice==4) makePayment();
            }
        }
    }
}