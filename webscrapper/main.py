#!/usr/bin/python3

# from fileinput import filename
import sys
import re
import random
import time

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By

def usage():
    print(f"{sys.argv[0]} searchlistfile")

if len(sys.argv) < 2:
    usage()
    sys.exit()

filename = sys.argv[1]

f = open(filename)
search_list = f.read().split('\n')
f.close()

url = "https://duckduckgo.com/?q="

driver = webdriver.Firefox()

time_range = [0.5,1.5]

for srch in search_list:
    if srch != '':
        driver.get(url + srch)
        time.sleep(random.uniform(time_range[0], time_range[1]))
        links = driver.find_elements(By.ID, "links")[0].find_elements(By.TAG_NAME, 'a')#[1].get_attribute('href')
        linkshref = []
        print(f"{len(links)}:\t{srch}:\n")
        for link in links:
            # driver.get(link.find_elements(By.TAG_NAME, 'a')[1].get_attribute('href'))
            try:
                _link = link.get_attribute('href')
            except:
                pass
            if ".pdf" not in _link and "duckduckgo" not in _link and _link not in linkshref:
                linkshref.append(_link)
                print(f"\t{_link}")
                driver.execute_script(f"window.open('{_link}')")
                driver.switch_to.window(driver.window_handles[1])
                try:
                    driver.get(_link)
                except:
                    continue
                time.sleep(random.uniform(time_range[0], time_range[1]))
                txttitle = '_'.join(re.sub("[^a-zA-Z0-9]+", "",driver.title.lower()).split(' '))
                txt = ""
                for p in driver.find_elements(By.TAG_NAME, "p"):
                    try:
                        txt += p.text
                    except:
                        pass
                f = open(f'txts/{txttitle}.txt', 'w')
                f.write(txt)
                f.close()
                driver.close()
                driver.switch_to.window(driver.window_handles[0])
                time.sleep(random.uniform(time_range[0], time_range[1]))
        time.sleep(random.uniform(time_range[0], time_range[1]))
driver.close()