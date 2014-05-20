# -*- coding: utf8 -*-

from tweepy.streaming import StreamListener
from tweepy import OAuthHandler
from tweepy import Stream


from tweepy.utils import import_simplejson
json = import_simplejson()


import psycopg2

from datetime import datetime


# Go to http://dev.twitter.com and create an app.
# The consumer key and secret will be generated for you after
consumer_key="hWN0t88Tj0sNuhZ25mXDxgVFD"
consumer_secret="CZ2clwPD8ndNnDRMZGoGkpWCboqYxx2IcC1mfPqEO5q1QNLB0v"

# After the step above, you will be redirected to your app's page.
# Create an access token under the the "Your access token" section
access_token="192064967-IKdTvcH0b847V738owAKhvRC9riXGBUnZ122RMc4"
access_token_secret="DZxqNQCCYqqOoHpXGbh4NkWx47r05MhQ0GYpqvABgphtT"


# logging
log_start = str(datetime.now())
pg_log = open('postgres.log', 'a')
pg_log.write('\n\n*** Starting logging at: %s ***\n\n' % log_start)
tweets_log = open('tweets.log', 'a')
tweets_log.write('\n\n*** Starting logging at: %s ***\n\n' % log_start)


def insert(user, tweet):
    
    try:
        conn_string = "dbname='eleito' user='eleito' password='eleito'"
        conn=psycopg2.connect(conn_string)
    except:
        pg_log.write("Unable to connect to the database.\n")
    
    cur = conn.cursor()
    insert_str = """INSERT INTO tweets(profile, name, location, tweet) 
                    VALUES ('%s', '%s', '%s', '%s') """ %(user['screen_name'],
                    user['name'], user['location'], unicode(tweet))
   
    try:
        pg_log.write("%s\n" % insert_str)
    except UnicodeEncodeError:
        pass
    except UnicodeDecodeError:
        pass

    try:
        cur.execute(insert_str)
        conn.commit()
    except Exception as e:
        # raise e
        pg_log.write("I can't insert data on database\n")




class StdOutListener(StreamListener):
    """ A listener handles tweets are the received from the stream.
    This is a basic listener that just prints received tweets to stdout.

    """
    def on_data(self, data):
        data = json.loads(data)
        user = data['user']
        tweet = data['text']
        
        insert(user, tweet)
        try:
            tweets_log.write("Person: %s - @%s\nFrom: %s\nTweet: %s\n" 
                             % (user['name'], user['screen_name'], 
                                user['location'],tweet))
        except UnicodeEncodeError:
            pass
        except UnicodeDecodeError:
            pass
        return True

    def on_error(self, status):
        print status

if __name__ == '__main__':
    l = StdOutListener()
    auth = OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)

    stream = Stream(auth, l)
    track_list = ['dilma','aecio','eduardo campos',
                  'Dilma Rousseff', u'Aécio Neves', 'Eduardo Campos',
                  u'eleições 2014']
    stream.filter(track=track_list)
