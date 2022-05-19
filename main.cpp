import requests

def getManip(itemName):
  bzReal = requests.get("https://api.hypixel.net/skyblock/bazaar")
  bzRealData = bzReal.json()
  write = open("Repeats.txt", "a")
  read = open("Repeats.txt", "r")
  response = requests.get("https://sky.coflnet.com/api/bazaar/" + itemName + "/history/day")
  webhook = 'https://discord.com/api/webhooks/976100292024217601/gp3-Fno4cVEGZ-G4A9vI1J63XL07MW5DleVm3RBY_25LVz_dyN0OoeHA0tVO0bB7zKB4'
  foundRepeat = False

  try:
    bzRealData['products'][itemName]
  except:
    print(itemName + " doesn't exist or isn't in the bazaar!")
    return 0
  
  try:
    bzItemData = response.json()
    
    totalBuy = 0
    totalSell = 0
    divideBy = 0
    sellFail = 0
    buyFail = 0
    
    for buy in bzItemData:
      try:
        totalBuy = totalBuy + buy['buy']
        divideBy += 1
      except:
        buyFail += 1
    
    for sell in bzItemData:
      try:
        totalSell = totalSell + sell['sell']
        divideBy += 1
      except:
        sellFail += 1
    try:
      average = (totalBuy + totalSell) / divideBy
      #Compare
      currentSell = bzRealData['products'][itemName]['buy_summary'][0]['pricePerUnit']
      print("\nAverage sell value of " + itemName + ": " + str("{:.1f}".format(average)) + "\nCurrent sell value of " + itemName + ": " + str("{:.1f}".format(currentSell)))

      #Repeats
      repeatsList = read.readlines()
      for i in repeatsList:
        i  = i.replace("\n", "")
        if str(i) == str(itemName):
          foundRepeat = True
          break

      if foundRepeat == False:
        if float(currentSell) > (float(average) * 3.5):
          if float(currentSell) > (float(average) * 10):
            data = {
              "content" : "<@&976279672281247775>",
              "username" : "BZ Logger"
            }
            data["embeds"] = [
              {
                  "description" : "Average: " + str("{:.1f}".format(average)) + "\nSell Offer: " + str("{:.1f}".format(currentSell)) + "\nInstant Sell: " + str(bzRealData['products'][itemName]['sell_summary'][0]['pricePerUnit']) + "\nProfit Percentage: " + str("{:.1f}".format((((float(currentSell)/float(average))*100)-100))) + "%",
                  "title" : itemName
              }
            ]
            requests.post(webhook, json = data)
          else:
            data = {
              "content" : "<@&976279629323194459>",
              "username" : "BZ Logger"
            }
            data["embeds"] = [
              {
                  "description" : "Average: " + str("{:.1f}".format(average)) + "\nSell Offer: " + str("{:.1f}".format(currentSell)) + "\nInstant Sell: " + str(bzRealData['products'][itemName]['sell_summary'][0]['pricePerUnit']) + "\nProfit Percentage (Sell offer): " + str("{:.1f}".format((((float(currentSell)/float(average))*100)-100))) + "%",
                  "title" : itemName
              }
            ]
            requests.post('https://discord.com/api/webhooks/976148398577168444/Sj1jJnQgzHQW5WIOONsnn1lVN0QID1MQ_64aRA9PtsNyRSmJr5uPtRXCTzG2vElvd0Lf', json = data)
          write.write(itemName + "\n")
      #Compare
      data = {
        "username" : "BZ Logger"
      }
      data["embeds"] = [
        {
          "description" : "Average: " + str("{:.1f}".format(average)) + "\nSell Offer: " + str("{:.1f}".format(currentSell)) + "\nInstant Sell: " + str(bzRealData['products'][itemName]['sell_summary'][0]['pricePerUnit']) + "\nProfit Percentage (Sell offer): " + str("{:.1f}".format((((float(currentSell)/float(average))*100)-100))) + "%",
          "title" : itemName
        }
      ]
      requests.post('https://discord.com/api/webhooks/976316263150276608/Pjgz19uO1RmMqLc8WwIvb9UesKulu21sQ-RBQpeeH5Qp7kveCZORu5Xf-KI_Cl-0voHw', json = data)
    except:
      print("An error has occurred calculating the average of " + itemName + ". There were " + str(sellFail) + " sell fails and " + str(buyFail) + " buy fails.")
  except:
    print("An error occured grabbing the average of " + itemName + " " + str(response.status_code()))
  write.close()
  read.close()

count = 0
response = requests.get("https://sky.coflnet.com/api/items/bazaar/tags")
bzItems = response.json()
while 1:
  for i in range(len(bzItems)):
    ranTrue = False
    while ranTrue == False:
      try:
        getManip(bzItems[i])
        ranTrue = True
      except:
        print("API call quota reached (100/minute)! Retrying.")
        ranTrue = False
  count += 1
  data = {
    "content" : "<@&976320781493628968>",
    "username" : "BZ Logger"
  }
  data["embeds"] = [
    {
        "description" : "Repeats: " + str(count),
        "title" : "Repeat Count"
    }
  ]
  requests.post("https://discord.com/api/webhooks/976316263150276608/Pjgz19uO1RmMqLc8WwIvb9UesKulu21sQ-RBQpeeH5Qp7kveCZORu5Xf-KI_Cl-0voHw", json = data)
  if count % 5 == 0:
    d = open("Repeats.txt", "w")
    d.close()
    count = 0
