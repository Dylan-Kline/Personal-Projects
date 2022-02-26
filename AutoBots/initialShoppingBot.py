import time

from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.common.keys import Keys

PATH = "webdriver path"
driver = webdriver.Firefox(executable_path=PATH)
driver.get("https://cannonkeys.com")

#Checks if item is in stock and if so adds to the cart
def stockCheck():

    refreshed = 0

    while (True):
        #Adding to Cart
        isAvail = driver.find_element_by_xpath("//*[@id='AddToCartText-product-template']").text
        print(isAvail)
        if (isAvail == 'ADD TO CART'):
            #Adding to cart
            addtocart = driver.find_element_by_xpath("//*[@id='AddToCart-product-template']")
            addtocart.click()
            return 1
        else:
            driver.refresh()
            print("Refreshed")
            time.sleep(5)
            refreshed += 1
            if (refreshed > 3): #Make this line a comment if you need infinite refreshs
                return 0 #This one as well

#Navigates to checkout
def checkOut():
    driver.find_element_by_xpath("//a[@href='/cart']").click()
    driver.implicitly_wait(2)
    driver.find_element_by_xpath("//input[@name='checkout']").click()
    driver.implicitly_wait(2)

#Fills in purchase info
def login(email, passWord):

    driver.find_element_by_xpath("//a[contains(@href, 'account/login')]").click()

    #Enter Email
    cusEmail = driver.find_element_by_xpath("//input[@id='CustomerEmail']")
    cusEmail.click()
    cusEmail.send_keys(email)

    #Enter password
    cusPass = driver.find_element_by_xpath("//input[contains(@id, 'CustomerPassword')]")
    cusPass.click()
    cusPass.send_keys(passWord)

    driver.find_element_by_xpath("//input[@value='Sign In']").click()

def paymentInfo(cardNum, cardName, expMM, expYY, security):

    #Card Info input
    wait = WebDriverWait(driver, 20)


    #Card number input
    wait.until(ec.frame_to_be_available_and_switch_to_it((By.CSS_SELECTOR, "iframe[id^='card-fields-number-']")))
    wait.until(ec.element_to_be_clickable((By.XPATH, "//input[@id='number' and @name='number']" ))).send_keys(cardNum)

    #Name on card input
    driver.switch_to.default_content()
    wait.until(ec.frame_to_be_available_and_switch_to_it((By.CSS_SELECTOR, "iframe[id^='card-fields-name']")))
    wait.until(ec.element_to_be_clickable((By.XPATH, "//input[@id='name' and @name='name']"))).send_keys(cardName)

    #Expiration Date and Cvv input
    driver.switch_to.default_content()
    wait.until(ec.frame_to_be_available_and_switch_to_it((By.CSS_SELECTOR, "iframe[id^='card-fields-expiry']")))
    expDate = driver.find_element_by_xpath("//input[@id='expiry' and @name='expiry']")
    expDate.click()

    expDate.send_keys(expMM)
    driver.implicitly_wait(2)
    expDate.click()
    expDate.send_keys(expYY, Keys.TAB, security)
    driver.switch_to.default_content()

def main():

    #Input checkout info
    email = 'testingbot@gmail.com'#input("Enter your login Email: ")
    passWord = 'Test123' #input("Enter your login password: ")
    cardNum = '1' #input("Enter Card Number: ")
    cardName = '2' #input("Enter Name on card: ")
    expMM = '03' #input("Enter Expiration date MM: ")
    expYY = '21' #input("Enter Expiration date YY: ")
    cvv = '444' #input("Enter Security code: ")
    baseAlphas = 0 #Alphas for keyset
    mods = 0 #mods for keyset
    novel = 0 #novelties for keyset
    novel2 = 0 #Secondary novelties for keyset

    #Navigating to the Hallyu page
    shop_button = driver.find_element_by_xpath("/html/body/div[2]/div/header/div/nav/ul/li[2]/button")
    shop_button.click()

    #Group buy page
    gb = driver.find_element_by_xpath("/html/body/div[2]/div/header/div/nav/ul/li[2]/div/ul/li[3]/a")
    gb.click()


    hallyu = driver.find_element_by_xpath("/html/body/div[3]/main/div/div/div/ul/li[2]/div/a")
    hallyu.click()

    #Option selector
    options = driver.find_element_by_xpath("//*[@id='SingleOptionSelector-0']")
    options.click()

    #Base alphas
    base = driver.find_element_by_xpath("/html/body/div[3]/main/div/div[1]/div/div[2]/div[1]/form/div[1]/select/option[1]")
    base.click()

    if (driver.find_element_by_xpath("//input[@id='confirm-check']")):
        agree = driver.find_element_by_xpath("/html/body/div[3]/main/div/div[1]/div/div[2]/div[1]/form/label")
        agree.click()
        driver.implicitly_wait(2)

    #Checks if base pack is available and adds to cart
    if (stockCheck() == 1):
        baseAlphas += 1

    options.click()

    #Checks if Novelties are available and adds to cart
    Novs = driver.find_element_by_xpath("/html/body/div[3]/main/div/div[1]/div/div[2]/div[1]/form/div[1]/select/option[2]")
    Novs.click()

    checker = stockCheck()
    if (checker == 1):
        novel += 1
    elif (checker == 0):
        novs2 = driver.find_element_by_xpath("/html/body/div[3]/main/div/div[1]/div/div[2]/div[1]/form/div[1]/select/option[8]")
        novs2.click()
        stockCheck()


    #Checkout
    if (baseAlphas == 1 or 0, novel == 1 or 0, novel2 == 1 or 0):
        checkOut()
        login(email, passWord)
        driver.implicitly_wait(5)
        driver.find_element_by_xpath("//button[@id='continue_button']").click()

        wait = WebDriverWait(driver, 10)
        wait.until(ec.element_to_be_clickable((By.XPATH, "//*[@id='continue_button']")))
        driver.find_element_by_xpath("//*[@id='continue_button']").click()

        driver.implicitly_wait(4)
        paymentInfo(cardNum,cardName,expMM, expYY,cvv)

        ##driver.find_element_by_xpath("//span[text()='Pay now'").click()



    time.sleep(10)

main()
