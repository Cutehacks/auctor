pragma Singleton
import QtQuick 2.0

OAuth2Provider {
    baseUrl: "https://slack.com"
    authUrl: baseUrl + "/oauth/authorize"
    tokenUrl: baseUrl + "/api/oauth.access"
}
