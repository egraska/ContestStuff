#include <stdio.h>
#include <time.h>
#include <string.h>
#include "game_parameters.h"
#include "resources.h"

void displayMarketValues(double market_values[], double market_demand[], int total_resources)
{
    char time_stamp[TIME_STAMP_SIZE];
    memset(time_stamp, 0, sizeof(time_stamp));
    time_t timeStamp;
    time(&timeStamp);
    struct tm myTime;
    localtime_r(&timeStamp, &myTime);
    strftime(time_stamp, TIME_STAMP_SIZE, SCORECARD_TIME_STAMP_FORMAT, &myTime);

    FILE *score;
    score = fopen (getMarketPath(), "w");

    if(score)
    {
        fprintf(score, 
                "<html>\n"
                "<head>\n"
                "<META http-equiv=\"Refresh\" CONTENT=\"10;url=%s%s\">\n"
                "<title>%s</title>\n"
                "</head>\n"
                "<body bgcolor=\"#ffffdf\">\n"
                "<center>\n"
                "<table cellspacing=10>\n"
                "<tr>\n"
                "<th><b><font size=+2 color=\"#BB0000\">%s</font></b></th>\n"
                "</tr>\n"
                "</table><p>\n"
                "<font size=+2 color=\"#0000BB\"><b><nobr>Market Value</nobr></b></font><p>\n"
                "<p><font size=-1 color=\"#cc0000\">%s</font><p>\n"
                "</center>\n"
                "<p>\n"
                "<center><p><a href = \"%s%s\">Scoreboard</a></p></center>"
                "<table align=\"center\">\n"
                "<tr>\n" 
                "<th><u>Resource</u></th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th>&nbsp</th>\n"
                "<th><u>Value</u></th>\n"
                "</tr>\n", getWebURL(), getMarketValueFile(), getScorecardTitle(), getScorecardTitle(), time_stamp, getWebURL(), getScorecardFile());

        for(int i=0; i<total_resources; i++)
        {
            if(market_demand[i] < 0)
            {
                fprintf(score,
                    "<tr style=\"text-align:center\"><td>%s</td><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><td>%lf</td><th>&nbsp</th><th>&nbsp</th><td><font color = \"Red\">%d</td></tr>\n", getResourceNameLowerCase(i), market_values[i], (int)market_demand[i]);
            }
            else
            {
                fprintf(score,
                    "<tr style=\"text-align:center\"><td>%s</td><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><th>&nbsp</th><td>%lf</td><th>&nbsp</th><th>&nbsp</th><td><font color = \"Green\">+%d</td></tr>\n", getResourceNameLowerCase(i), market_values[i], (int)market_demand[i]);
            }
        }

        fprintf(score,
                "</table>\n"
                "</body>\n"
                "</html>\n");
        fclose(score);
    }
}

void initializeMarketWebpage()
{
    int total_resources = getTotalResources();
    double market_values[total_resources];
    double market_demand[total_resources];
    for(int i=0; i<total_resources; i++)
    {
        market_values[i] = getResourceValue(i);
        market_demand[i] = getResourceDemand(i);
    }
    displayMarketValues(market_values, market_demand, TOTAL_RESOURCES);
}

void refreshMarketValues()
{
    int total_resources = getTotalResources();
    double market_values[total_resources];
    double market_demand[total_resources];
    getMarket(market_values, market_demand);
    displayMarketValues(market_values, market_demand, TOTAL_RESOURCES);
}
