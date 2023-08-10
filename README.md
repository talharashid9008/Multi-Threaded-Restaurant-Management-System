# Multi-Threaded-Restaurant-Management-System

The project contains the creation of structures for order, item, and manager, along with a struct for customer. A menu comprising five items - "burger, pizza, sandwich, fries, and pasta" - has been established. Implementation includes the utilization of threads for both the cook and waiter roles, which are subsequently employed in the system.

Communication between the manager and customer is facilitated using unnamed pipes. In this architecture, the manager operates as the child process, while the customer functions as the parent process within the fork call. The initial interaction involves collecting the user's name and ID, followed by the concatenation of these values within the child process and their subsequent separation in the parent process using pipes.

Orders are represented as vectors of items. Upon order placement, the order is then directed to a dedicated cook thread, which ensures the preparation of the requested items within specified timeframes. Once the order is prepared, it is handed over to a waiter thread, whose responsibility is to deliver the order promptly to the respective customer. This multithreaded approach enhances the efficiency of order processing and delivery.

Overall, the project demonstrates a comprehensive implementation encompassing various components, such as process management, inter-process communication, data structuring, and multithreaded execution, to provide a functional and organized system for handling customer orders within a restaurant setting.
