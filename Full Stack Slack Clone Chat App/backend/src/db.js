import pg from 'pg';
import jwt from 'jsonwebtoken';
import dotenv from 'dotenv';

dotenv.config();

const pool = new pg.Pool({
  host: 'localhost',
  port: 5432,
  database: process.env.POSTGRES_DB,
  user: process.env.POSTGRES_USER,
  password: process.env.POSTGRES_PASSWORD,
});

/**
 * Authenticate user against the database
 * @param {string} email - User email
 * @param {string} password - User password (plaintext)
 * @returns {Promise<{user: object, token: string} | null>} -
 */
export async function authenticateUser(email, password) {
  // Query to check if user exists and password matches
  const result = await pool.query(`
    SELECT 
      id,
      data->>'email' AS email,
      data->>'name' AS name,
      data->>'role' AS role
    FROM users
    WHERE 
      data->>'email' = $1 AND
      (data->>'password')::text = crypt($2, (data->>'password')::text)
  `, [email, password]);

  // If no user found or password doesn't match
  if (result.rows.length === 0) {
    return null;
  }

  const user = result.rows[0];

  // Generate JWT token
  const token = jwt.sign(
      {
        id: user.id,
        name: user.name,
        email: user.email,
        role: user.role,
      },
      process.env.JWT_SECRET || 'your-secret-key',
      {expiresIn: '24h'},
  );

  return {
    token,
  };
}

/**
 * Get workspaces for a user by name
 * @param {string} username - User's name
 * @returns {Promise<Array>} list of workspaces
 */
export async function userWorkspaces(username) {
  // First query to get the user ID from the name
  const userResult = await pool.query(`
      SELECT
        id
      FROM users
      WHERE data->>'name' = $1
    `, [username]);
  const userID = userResult.rows[0].id;

  // Second query to get the workspaces for this user ID
  const workspacesResult = await pool.query(`
      SELECT 
        workspaces.id AS workspace_id,
        data->>'name' AS name
      FROM user_workspaces
      JOIN workspaces ON user_workspaces.workspace_id = workspaces.id
      WHERE user_workspaces.user_id = $1
    `, [userID]);

  return workspacesResult.rows;
}

/**
 * Get workspaces for a user by name
 * @param {string} workspace - User's name
 * @returns {Promise<Array>} list of workspaces
 */
export async function selectChannels(workspace) {
  // First query to get the user ID from the name
  const workspaceResult = await pool.query(`
      SELECT
        id
      FROM workspaces
      WHERE data->>'name' = $1
    `, [workspace]);
  const workspaceID = workspaceResult.rows[0].id;

  // Second query to get the workspaces for this user ID
  const channelsResult = await pool.query(`
      SELECT 
        channels.id,
        data->>'name' AS name,
      data->>'message' AS message
      FROM workspace_channels
      JOIN channels ON workspace_channels.channel_id = channels.id
      WHERE workspace_channels.workspace_id = $1
    `, [workspaceID]);

  return channelsResult.rows;
}

/**
 * Get messages for a specific channel by ID with user names
 * @param {string} channelId - The UUID of the channel
 * @returns {Promise<object>} Channel data including messages with user names
 */
export async function getChannelById(channelId) {
  // First, get the channel data
  const channelResult = await pool.query(`
      SELECT 
        id,
        data
      FROM channels
      WHERE id = $1
    `, [channelId]);

  const channelData = channelResult.rows[0].data;


  // Get all unique user IDs from messages
  const userIds = [...new Set(channelData
      .messages.map((msg) => msg.user_id))];

  // Query to get usernames for all user IDs
  const userResult = await pool.query(`
      SELECT 
        id,
        data->>'name' AS name
      FROM users
      WHERE id = ANY($1::uuid[])
    `, [userIds]);

  // Create a map of user IDs to names
  const userMap = {};
  userResult.rows.forEach((user) => {
    userMap[user.id] = user.name;
  });

  // Enhance messages with user names
  const enhancedMessages = channelData.messages.map((msg) => {
    return {
      ...msg,
      username: userMap[msg.user_id],
    };
  });

  return {
    ...channelData,
    messages: enhancedMessages,
  };
}

/**
 * Check if a channel belongs to any of the given workspaces
 * @param {string} channelId - The UUID of the channel
 * @param {Array<string>} workspaceIds - Array of workspace IDs
 * @returns {Promise<boolean>} Whether the channel belongs
 */
export async function channelBelongsToUserWorkspaces(channelId, workspaceIds) {
  const result = await pool.query(`
    SELECT 1
    FROM workspace_channels
    WHERE channel_id = $1 AND workspace_id = ANY($2::uuid[])
    LIMIT 1
  `, [channelId, workspaceIds]);

  return result.rows.length > 0;
}

/**
 *
 * @param {any} workspaceName name
 * @returns {any} any
 */
export async function getWorkspaceOwners(workspaceName) {
  const workspaceResult = await pool.query(`
    SELECT
      data->>'owner_name' AS name
    FROM workspaces
    WHERE data->>'name' = $1
  `, [workspaceName]);

  const ownerData = workspaceResult.rows[0];
  return {
    name: ownerData.name,
  };
}
