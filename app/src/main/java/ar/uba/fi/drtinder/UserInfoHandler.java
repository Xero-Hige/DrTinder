package ar.uba.fi.drtinder;

import org.springframework.web.client.RestTemplate;

import java.io.StringWriter;

import au.com.bytecode.opencsv.CSVWriter;

/**
 * ${FILE}
 * <p>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses
 */

//TODO: Name
public class UserInfoHandler {

    public static String NULL_TOKEN = "";
    private static String LOGIN_URL = "";

    static String getLoginToken(String username, String password, String location) {

        RestTemplate restTemplate = new RestTemplate();

        String url = LOGIN_URL + username;

        String params[] = {username, password, location};
        StringWriter stringWriter = new StringWriter();
        CSVWriter writer = new CSVWriter(stringWriter);
        writer.writeNext(params);

        String response = " ";//restTemplate.postForObject(stringWriter.toString(), url, String.class);

        if (!response.equals("")) { //TODO Check
            return response;
        }
        return NULL_TOKEN;
    }
}
