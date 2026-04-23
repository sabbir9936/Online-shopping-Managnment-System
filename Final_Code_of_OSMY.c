#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
//---------------- PRODUCT DATA ----------------
int ids[MAX], qtys[MAX], totalProducts = 0;
char names[MAX][50];
float prices[MAX];
//---------------- ORDER DATA ----------------
int orderProductId[MAX];
int orderQty[MAX];
int totalOrders = 0;
//---------------- BUBBLE SORT ----------------
void bubbleSort() {
    for (int i = 0; i < totalProducts - 1; i++) {
        for (int j = 0; j < totalProducts - i - 1; j++) {

            if (prices[j] > prices[j + 1]) {

                float tp = prices[j];
                prices[j] = prices[j + 1];
                prices[j + 1] = tp;

                int ti = ids[j];
                ids[j] = ids[j + 1];
                ids[j + 1] = ti;

                int tq = qtys[j];
                qtys[j] = qtys[j + 1];
                qtys[j + 1] = tq;

                char tn[50];
                strcpy(tn, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], tn);
            }
        }
    }
}
//---------------- FILE ----------------
void saveToFile() {
    FILE *fp = fopen("products.txt", "w");
    if (!fp) {
        printf("Error saving file!\n");
        return;
    }

    for (int i = 0; i < totalProducts; i++) {
        fprintf(fp, "%d %s %.2f %d\n",
                ids[i], names[i], prices[i], qtys[i]);
    }

    fclose(fp);
}
void loadFromFile() {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) return;
    while (totalProducts < MAX &&
           fscanf(fp, "%d %s %f %d",
                  &ids[totalProducts],
                  names[totalProducts],
                  &prices[totalProducts],
                  &qtys[totalProducts]) == 4) {
        totalProducts++;
    }

    fclose(fp);
}
//---------------- SEARCH ----------------
int findProduct(int id) {
    for (int i = 0; i < totalProducts; i++) {
        if (ids[i] == id)
            return i;
    }
    return -1;
}
//---------------- ADMIN FUNCTIONS ----------------
void addProduct() {
    if (totalProducts >= MAX) {
        printf("Storage Full!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &ids[totalProducts]);

    if (findProduct(ids[totalProducts]) != -1) {
        printf("ID already exists!\n");
        return;
    }
    printf("Enter Name: ");
    scanf("%s", names[totalProducts]);
    printf("Enter Price: ");
    scanf("%f", &prices[totalProducts]);
    printf("Enter Quantity: ");
    scanf("%d", &qtys[totalProducts]);
    totalProducts++;
    saveToFile();
    printf("Product Added Successfully!\n");
}
void deleteProduct() {
    int id;
    printf("Enter ID to Delete: ");
    scanf("%d", &id);
    int index = findProduct(id);
    if (index == -1) {
        printf("Product Not Found!\n");
        return;
    }
    for (int i = index; i < totalProducts - 1; i++) {
        ids[i] = ids[i + 1];
        strcpy(names[i], names[i + 1]);
        prices[i] = prices[i + 1];
        qtys[i] = qtys[i + 1];
    }
    totalProducts--;
    saveToFile();
    printf("Product Deleted Successfully!\n");
}
void updateProduct() {
    int id;
    printf("Enter ID to Update: ");
    scanf("%d", &id);

    int i = findProduct(id);

    if (i == -1) {
        printf("Product Not Found!\n");
        return;
    }

    printf("New Name: ");
    scanf("%s", names[i]);

    printf("New Price: ");
    scanf("%f", &prices[i]);

    printf("New Quantity: ");
    scanf("%d", &qtys[i]);

    saveToFile();
    printf("Product Updated Successfully!\n");
}

//---------------- DISPLAY ----------------
void displayProducts() {
    if (totalProducts == 0) {
        printf("No Products Available!\n");
        return;
    }

    bubbleSort();   //Sorting added here

    printf("\n----------------------------------------------------------\n");
    printf("| %-5s | %-15s | %-10s | %-8s |\n", "ID", "Name", "Price", "Qty");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < totalProducts; i++) {
        printf("| %-5d | %-15s | %-10.2f | %-8d |\n",
               ids[i], names[i], prices[i], qtys[i]);
    }

    printf("----------------------------------------------------------\n");
}

//---------------- SEARCH ----------------
void searchProduct() {
    int id;
    printf("Enter Product ID: ");
    scanf("%d", &id);

    int i = findProduct(id);

    if (i == -1) {
        printf("Product Not Found!\n");
        return;
    }

    printf("\n----- Product Details -----\n");
    printf("ID       : %d\n", ids[i]);
    printf("Name     : %s\n", names[i]);
    printf("Price    : %.2f\n", prices[i]);
    printf("Quantity : %d\n", qtys[i]);
}

//---------------- ORDER ----------------
void orderProduct() {
    int id, qty;
    printf("Enter Product ID: ");
    scanf("%d", &id);
    int i = findProduct(id);
    if (i == -1) {
        printf("Product Not Found!\n");
        return;
    }

    printf("Enter Quantity: ");
    scanf("%d", &qty);

    if (qty <= 0 || qtys[i] < qty) {
        printf("Invalid Quantity or Insufficient Stock!\n");
        return;
    }
    qtys[i] -= qty;
    orderProductId[totalOrders] = id;
    orderQty[totalOrders] = qty;
    totalOrders++;
    saveToFile();
    printf("Order Placed Successfully!\n");
}
//---------------- VIEW ORDERS ----------------
void viewOrders() {
    if (totalOrders == 0) {
        printf("No Orders Found!\n");
        return;
    }

    printf("\n------ ORDER HISTORY ------\n");

    for (int i = 0; i < totalOrders; i++) {
        printf("Index: %d | Product ID: %d | Quantity: %d\n",
               i, orderProductId[i], orderQty[i]);
    }
}

//---------------- DELETE ORDER ----------------
void deleteOrder() {
    int index;

    if (totalOrders == 0) {
        printf("No Orders Found!\n");
        return;
    }

    printf("Enter Order Index: ");
    scanf("%d", &index);

    if (index < 0 || index >= totalOrders) {
        printf("Invalid Index!\n");
        return;
    }

    int id = orderProductId[index];
    int qty = orderQty[index];

    int i = findProduct(id);
    if (i != -1) {
        qtys[i] += qty;
    }

    for (int j = index; j < totalOrders - 1; j++) {
        orderProductId[j] = orderProductId[j + 1];
        orderQty[j] = orderQty[j + 1];
    }

    totalOrders--;

    printf("Order Deleted Successfully!\n");
}

//---------------- ADMIN MENU ----------------
void adminMenu() {
    int ch;

    while (1) {
        printf("\n------ ADMIN MENU ------\n");
        printf("1. Add Product\n");
        printf("2. Delete Product\n");
        printf("3. Update Product\n");
        printf("4. Search Product\n");
        printf("5. View All Products\n");
        printf("6. View Orders\n");
        printf("7. Back\n");
        printf("Enter Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addProduct(); break;
            case 2: deleteProduct(); break;
            case 3: updateProduct(); break;
            case 4: searchProduct(); break;
            case 5: displayProducts(); break;
            case 6: viewOrders(); break;
            case 7: return;
            default: printf("Invalid Choice!\n");
        }
    }
}

//---------------- USER MENU ----------------
void userMenu() {
    int ch;

    while (1) {
        printf("\n------ USER MENU ------\n");
        printf("1. Search Product\n");
        printf("2. View All Products\n");
        printf("3. Order Product\n");
        printf("4. View Orders\n");
        printf("5. Delete Order\n");
        printf("6. Back\n");
        printf("Enter Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: searchProduct(); break;
            case 2: displayProducts(); break;
            case 3: orderProduct(); break;
            case 4: viewOrders(); break;
            case 5: deleteOrder(); break;
            case 6: return;
            default: printf("Invalid Choice!\n");
        }
    }
}

//---------------- MAIN ----------------
int main() {
    loadFromFile();

    int ch;

    while (1) {
        printf("\n===== ONLINE SHOPPING SYSTEM =====\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: adminMenu(); break;
            case 2: userMenu(); break;
            case 3: printf("Goodbye!\n"); exit(0);
            default: printf("Invalid Choice!\n");
        }
    }

    return 0;
}