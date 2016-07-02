package ar.uba.fi.drtinder;

import android.graphics.Bitmap;
import android.net.Uri;
import android.util.Base64;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;

import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.http.converter.StringHttpMessageConverter;
import org.springframework.web.client.HttpClientErrorException;
import org.springframework.web.client.HttpServerErrorException;
import org.springframework.web.client.ResourceAccessException;
import org.springframework.web.client.RestTemplate;

import java.io.ByteArrayOutputStream;
import java.io.StringWriter;
import java.util.Locale;
import java.util.Map;

import au.com.bytecode.opencsv.CSVWriter;


/**
 * @author Xero-Hige
 * Copyright 2016 Gaston Martinez Gaston.martinez.90@gmail.com
 * <p>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 * Class that handles user specific task such as login or profile update
 */
public final class UserHandler {

    /**
     * Result Token: Failed to get
     */
    public static final String FAILED_TOKEN = "-";
    /**
     * Result Token: Error occurred while fetching
     */
    public static final String ERROR_TOKEN = "";

    /**
     * Sign up result: User already exists
     */
    public static final String SIGNUP_USEREXIST = "E";
    /**
     * Sign up result: Sign up failed
     */
    public static final String SIGNUP_FAILED = "F";
    /**
     * Sign up result: Sign up successful
     */
    public static final String SIGNUP_SUCCESS = "S";
    private static final String LOGIN_URL = "user";
    private static final String DELETE_URL = "users";
    private static final String SIGNUP_URL = "users";
    private static final String UPDATE_URL = "users";
    private static final String AVATAR_URL = "users/photo";
    private static String mToken = ERROR_TOKEN;

    private UserHandler() {
    }

    /**
     * Fetches token from server and returns it
     *
     * @param email    User email
     * @param password User password
     * @param location User location
     * @return Token string. If error, one of the listed error tokens
     */
    static String getLoginToken(String email, String password, String location) {

        RestTemplate restTemplate = new RestTemplate();

        String user = getUsernameFrom(email);

        HttpHeaders requestHeaders = new HttpHeaders();
        addAuthHeader(password, user, requestHeaders);

        String body = String.format(Locale.ENGLISH, "localization=%s", location);

        HttpEntity<?> requestEntity = new HttpEntity<>(body, requestHeaders);

        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());

        ResponseEntity<String> response;

        try {
            response = restTemplate.exchange(getLoginUrl(), HttpMethod.POST,
                    requestEntity, String.class);
        } catch (HttpServerErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
            mToken = ERROR_TOKEN;
            return mToken;
        } catch (HttpClientErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Client error: " + e.getMessage());
            mToken = ERROR_TOKEN;
            return mToken;
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_WARN, "Failed to connect: " + e.getMessage());
            mToken = ERROR_TOKEN;
            return mToken;
        }

        int statusCode = response.getStatusCode().value();

        if (statusCode != 200) {
            if (statusCode == 401) {
                mToken = FAILED_TOKEN;
                return FAILED_TOKEN;
            }
            String errorMessage = "Failed login post: "
                    + response.getStatusCode().value()
                    + " " + response.getStatusCode().getReasonPhrase();
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, errorMessage);
        }

        mToken = response.getBody();
        return response.getBody();


//        restTemplate = new RestTemplate();
//
//        String tokenUrl = getTokenUrl() + "/" + user;
//        response = restTemplate.getForEntity(tokenUrl, String.class);
//
//
//        return mToken;
    }

    private static String getLoginUrl() {
        return ServerUrlWrapper.getServerUrl() + LOGIN_URL;
    }

    private static String getUsernameFrom(String email) {
        String[] fields = email.split("@");
        return fields[0] + fields[1].replace(".", "");
        //return email;
    }

    private static void addAuthHeader(String password, String user, HttpHeaders requestHeaders) {
        requestHeaders.add("Authorization", String.format(Locale.ENGLISH,
                "Authorization := username=\"%s\" pass=\"%s\"", user, password));
    }

    /**
     * Validates password, with the global rules
     *
     * @param pass Candidate password
     * @return true if its valid, false otherwise
     */
    public static boolean isValidPassword(String pass) {
        return pass.length() >= 6;
    }

    /**
     * Validates email, with the global rules
     *
     * @param email Candidate email
     * @return true if its valid, false otherwise
     */
    public static boolean isValidEmail(String email) {
        return email.matches("[^@]*@[^.]*\\....?\\.?.?.?");
    }

    /**
     * Gets logged user username (id)
     *
     * @return Logged user username
     */
    public static String getUsername() {
        if (!isLoggedIn()) {
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Not logged in fetching username");
            return "";
        }

        return getUsernameFrom(getUserEmail());
    }

    /**
     * Checks if the system is logged in
     *
     * @return true if there is a logged in session
     */
    public static boolean isLoggedIn() {
        return FirebaseAuth.getInstance().getCurrentUser() != null;
    }

    /**
     * Gets logged user email
     *
     * @return Logged user email
     */
    public static String getUserEmail() {
        if (!isLoggedIn()) {
            DrTinderLogger.writeLog(DrTinderLogger.ERRO, "Not logged in fetching email");
            return "";
        }

        FirebaseUser currentUser = FirebaseAuth.getInstance().getCurrentUser();
        assert currentUser != null; //DEBUG Assert
        return currentUser.getEmail();
    }

    /**
     * Logouts from the current session
     */
    public static void logout() {
        FirebaseAuth.getInstance().signOut();
    }

    /**
     * Deletes user profile of the current logged in user
     *
     * @param token Session token
     * @return true if success
     */
    public static boolean deleteProfile(String token) {
        Uri.Builder uriBuilder = Uri.parse(getDeleteUrl()).buildUpon();
        uriBuilder.appendQueryParameter("token", token);
        String deleteUrl = uriBuilder.build().toString();

        RestTemplate restTemplate = new RestTemplate();
        try {
            restTemplate.delete(deleteUrl);
        } catch (HttpServerErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
        } catch (HttpClientErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Client error: " + e.getMessage());
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_WARN, "Failed to connect: " + e.getMessage());
        }
        FirebaseAuth.getInstance().signOut();
        return true;
    }

    private static String getDeleteUrl() {
        return ServerUrlWrapper.getServerUrl() + DELETE_URL;
    }

    /**
     * Creates a new user with the info
     *
     * @param email    User email
     * @param password User password
     * @param userdata Map containing userdata as field:value
     * @return Sign up result (one of the listed results const)
     */
    public static String signUp(String email, String password, Map<String, String> userdata) {

        RestTemplate restTemplate = new RestTemplate();
        String user = getUsernameFrom(email);

        HttpHeaders requestHeaders = new HttpHeaders();
        addAuthHeader(password, user, requestHeaders);

        String name = userdata.get("name");
        String age = userdata.get("age");
        String sex = userdata.get("sex");
        String interest = userdata.get("interest");

        StringWriter sWriter = new StringWriter();
        CSVWriter writer = new CSVWriter(sWriter, ',');
        String[] line = {name, age, user, email, sex, " ", interest};
        writer.writeNext(line);
        String body = "User=" + sWriter.toString();

        HttpEntity<?> requestEntity = new HttpEntity<>(body, requestHeaders);

        restTemplate.getMessageConverters().add(new StringHttpMessageConverter());

        ResponseEntity<String> response;

        try {
            response = restTemplate.exchange(getSignupUrl(), HttpMethod.POST,
                    requestEntity, String.class);
        } catch (HttpServerErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
            return SIGNUP_FAILED;
        } catch (HttpClientErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Client error: " + e.getMessage());
            return SIGNUP_FAILED;
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_WARN, "Failed to connect: " + e.getMessage());
            return SIGNUP_FAILED;
        }

        int statusCode = response.getStatusCode().value();

        if (statusCode != 201) {
            if (statusCode == 401) {
                return SIGNUP_USEREXIST;
            }
            String errorMessage = "Failed login post: "
                    + response.getStatusCode().value()
                    + " " + response.getStatusCode().getReasonPhrase();
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, errorMessage);
            return SIGNUP_FAILED;
        }

        mToken = response.getBody();
        return SIGNUP_SUCCESS;
    }

    private static String getSignupUrl() {
        return ServerUrlWrapper.getServerUrl() + SIGNUP_URL;
    }

    /**
     * Updates user info
     *
     * @param token    Session token
     * @param userdata Map containing userdata as field:value
     * @return true if success
     */
    public static boolean updateInfo(String token, Map<String, String> userdata) {

        RestTemplate restTemplate = new RestTemplate();

        String name = userdata.get("name");
        String age = userdata.get("age");
        String sex = userdata.get("sex");
        String interest = userdata.get("interest");

        StringWriter sWriter = new StringWriter();
        CSVWriter writer = new CSVWriter(sWriter, ',');
        String[] line = {name, age, sex, interest};
        writer.writeNext(line);
        String body = "User=" + sWriter.toString();

        Uri.Builder uriBuilder = Uri.parse(getUpdateUrl()).buildUpon();
        uriBuilder.appendQueryParameter("token", token);
        String updateUrl = uriBuilder.build().toString();

        try {
            restTemplate.put(updateUrl, body);
        } catch (Exception e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Exception on update: " + e.getMessage());
            return false;
        }
        return true;
    }

    private static String getUpdateUrl() {
        return ServerUrlWrapper.getServerUrl() + UPDATE_URL;
    }

    /**
     * Uploads a new profile picture for the logged in user
     *
     * @param profilePicture New picture
     * @param token          Session token
     */
    public static void uploadProfilePicture(Bitmap profilePicture, String token) {
        RestTemplate restTemplate = new RestTemplate();

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        profilePicture.compress(Bitmap.CompressFormat.PNG, 100, byteArrayOutputStream);
        byte[] byteArray = byteArrayOutputStream.toByteArray();
        String body = Base64.encodeToString(byteArray, Base64.DEFAULT);

        Uri.Builder uriBuilder = Uri.parse(getAvatarUrl()).buildUpon();
        uriBuilder.appendQueryParameter("token", token);
        String updateUrl = uriBuilder.build().toString();

        try {
            restTemplate.postForEntity(updateUrl, body, String.class);
        } catch (HttpServerErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Server error: " + e.getMessage());
        } catch (HttpClientErrorException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Client error: " + e.getMessage());
        } catch (ResourceAccessException e) {
            DrTinderLogger.writeLog(DrTinderLogger.NET_ERRO, "Failed to connect: " + e.getMessage());
        }
    }

    private static String getAvatarUrl() {
        return ServerUrlWrapper.getServerUrl() + AVATAR_URL;
    }

    /**
     * Get the current session token
     *
     * @return The current session token. If not possible, one of the listed Error tokens
     */
    public static String getToken() {
        return mToken;
    }
}
