# atm-management-system

This is a simple **ATM Management System** written in C that uses **`.txt` files** to store data instead of a traditional database. It enables users to manage their bank accounts through various features like registration, transactions, updates, and more.

## Installation

To install my-ls-1, clone the repository and build the project:

```bash
git clone https://github.com/ombima56/atm-management-system.git
cd atm-management-system
```

## Data File Format
### 📄 `users.txt`

id name password
0 Alice 1234password
1 Michel password1234

### `records.txt`
id user_id username account_id creation_date country phone balance account_type
0 0 Alice 0 10/02/2020 german 986134231 11090830.00 current
1 1 Michel 2 10/10/2021 portugal 914134431 1920.42 savings
2 0 Alice 1 10/10/2000 finland 986134231 1234.21 savings

## Usage
Run the utility from the command line with the following steps:

### Build the Project
Run the following in terminal:

```bash
make
```

### Run the App
```sh
./atm
```
