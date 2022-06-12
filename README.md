# Introduction

This project aims to control output LEDs, a buzzer and an LCD based on the measured temperature from the sensor which is replaced by the potentiometer. Data was passed using queues to send data back and forth through tasks and additional help from UART was used to communicate between the user and the program itself. 

If the measured temperature is below the setpoint, the green LED (the heater) will light up until they are equal again, and the red LED will light up when the temperature is above 50 °C and turns off when it’s sub 50 °C. The buzzer also notifies the user when the measured temperature is above 50 °C alongside the red LED

## List of Inputs
-	Potentiometer (measured temperature) 
- User’s setpoint temperature

## List of Outputs
-	LCD Display of temperatures
-	2 LEDs: Green (heater) & Red (alarm)
-	Buzzer
## Task Diagram

![image](https://user-images.githubusercontent.com/70348804/173251551-0f38a02e-bfdc-4af3-9cf5-5a78d05f0d23.png)


## Flowcharts
| ![image](https://user-images.githubusercontent.com/70348804/173250557-e685b485-616f-4306-98cd-800045c3505c.png)  | ![image](https://user-images.githubusercontent.com/70348804/173250676-43487c1c-07f9-4a63-9225-0b6640290bc7.png) |
| :-------------: | :-------------: |
| ![image](https://user-images.githubusercontent.com/70348804/173250736-7927fb8a-f906-49f0-a8ae-0aff6660cf3a.png) | ![image](https://user-images.githubusercontent.com/70348804/173250712-328dbacb-b740-4ab6-b850-5cf42cd7a23b.png) |


## Description of files and tasks

### Files
| File Name | Description |
| :---: | :--- |
| main.c | Main file including 4 tasks, a structure, 4 functions (toString(), printchar(), print(), GPIO_init() |
| LCD_config.c | A library file for implementing LCD functions |
| ADC.c | A library file for initializing ADC |

### Tasks

| Task Name | Description |
| :---: | :--- |
| Task 1 | Main function which takes the measured temperature using ADC and compares it with the user’s setpoint and then turns the LEDs and the buzzer ON/OFF using the queues methods and sends the setpoint and measured temperature values to Task 3 which receives these values and displays them on the LCD. |
| Task 2 | Takes the setpoint temperature from the user through UART and sends it to Task 1 through a queue |
| Task 3 | Receives the setpoint and measured temperatures from Task 1 and displays them on the LCD |
| Task 4 | Receives the buzzer state from Task 1 and turns it ON/OFF |

## Hardware Connections with TivaC
![image](https://user-images.githubusercontent.com/70348804/173251083-6f7d4505-d5f7-481e-8828-36b286ddf54b.png)

## Output
| LCD Output  | Putty Output |
| :-------------: | :-------------: |
| ![image](https://user-images.githubusercontent.com/70348804/173251137-ad01bac7-f046-4022-aa97-86c6889226b9.png) | ![image](https://user-images.githubusercontent.com/70348804/173251139-b33316f0-3e44-427a-970a-2dcd5def7d24.png) |

## Running Images
| Image | Description |
| :---: | :---: |
| ![image](https://user-images.githubusercontent.com/70348804/173251198-86bbbe3e-3d37-42e7-8f7c-0b0665ebf528.png) | Green LED is on when the measured temperature is lower than the setpoint |
| ![image](https://user-images.githubusercontent.com/70348804/173251225-8b240ca0-c29c-4b70-8748-941b5ba7cf0a.png) | No LEDs are on when the measured temperature is greater than the setpoint but still sub 50°C |
| ![image](https://user-images.githubusercontent.com/70348804/173251234-648d5616-7ba9-46bd-94f9-a0b7c24b95e0.png) | Red LED is on when the measured temperature is greater than 50 °C |
