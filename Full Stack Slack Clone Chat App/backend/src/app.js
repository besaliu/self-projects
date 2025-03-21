/*
#######################################################################
#
# Copyright (C) 2020-2025 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without
# the express written permission of the copyright holder.
#
#######################################################################
*/
import express from 'express';
import cors from 'cors';
import yaml from 'js-yaml';
import swaggerUi from 'swagger-ui-express';
import fs from 'fs';
import path from 'node:path';
import OpenApiValidator from 'express-openapi-validator';
import {fileURLToPath} from 'node:url';
import * as auth from './routes/auth.js';
import {authenticateJWT} from './middleware/auth.js';
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
app.use(express.json());
app.use(express.urlencoded({extended: false}));

const apiSpec = path.join(__dirname, '../api/openapi.yaml');

const apidoc = yaml.load(fs.readFileSync(apiSpec, 'utf8'));
app.use('/api/v0/docs', swaggerUi.serve, swaggerUi.setup(apidoc));

// Allow connections from a non common origin so the UI can connect
app.use(cors({origin: 'http://localhost:3000'}));

app.use(
    OpenApiValidator.middleware({
      apiSpec: apiSpec,
      validateRequests: true,
      validateResponses: true,
    }),
);

// Your routes go here; however, do NOT write then inline.
// Create additional modules and delegate to their exports.

app.post('/api/v0/login', auth.userLogin);

app.get('/api/v0/workspace', authenticateJWT, auth.getWorkspaces);
app.get('/api/v0/workspace/owner', authenticateJWT, auth.getWorkspaceOwner);
app.get('/api/v0/channel', authenticateJWT, auth.getChannels);
app.get('/api/v0/channel/:id', authenticateJWT, auth.getChannelById);
app.use((err, req, res, next) => {
  res.status(err.status).json({
    message: err.message,
    errors: err.errors,
    status: err.status,
  });
});

export default app;

